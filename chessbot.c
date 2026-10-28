#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MOVES 256
#define INF 1000000

typedef struct {
    char b[64];
    int side; /* 1 for White, -1 for Black */
} Position;

typedef struct {
    int from;
    int to;
    char promo;
} Move;

static const char startBoard[64] = {
    'r','n','b','q','k','b','n','r',
    'p','p','p','p','p','p','p','p',
    '.','.','.','.','.','.','.','.',
    '.','.','.','.','.','.','.','.',
    '.','.','.','.','.','.','.','.',
    '.','.','.','.','.','.','.','.',
    'P','P','P','P','P','P','P','P',
    'R','N','B','Q','K','B','N','R'
};

static inline int idx(int r, int c) {
    return r * 8 + c;
}

static inline int sign(int x) {
    return (x > 0) - (x < 0);
}

static inline int color(char pc) {
    if (pc >= 'A' && pc <= 'Z') return 1;
    if (pc >= 'a' && pc <= 'z') return -1;
    return 0;
}

static inline int value(char pc) {
    char t = (char)tolower((unsigned char)pc);
    switch (t) {
        case 'p': return 100;
        case 'n': return 320;
        case 'b': return 330;
        case 'r': return 500;
        case 'q': return 900;
        case 'k': return 20000;
        default: return 0;
    }
}

int pathClear(Position *p, int from, int to) {
    int r = from / 8, c = from % 8, tr = to / 8, tc = to % 8, sr = sign(tr - r), sc = sign(tc - c);
    r += sr; c += sc;
    while (r != tr || c != tc) {
        if (p->b[idx(r, c)] != '.') return 0;
        r += sr; c += sc;
    }
    return 1;
}

int pseudoLegal(Position *p, int from, int to, char *promo) {
    if (from < 0 || from >= 64 || to < 0 || to >= 64 || from == to) return 0;
    char pc = p->b[from], dest = p->b[to];
    if (pc == '.' || color(pc) != p->side || color(dest) == p->side) return 0;
    int r = from / 8, c = from % 8, tr = to / 8, tc = to % 8, dr = tr - r, dc = tc - c;
    char t = (char)tolower((unsigned char)pc);
    *promo = 0;
    if (t == 'p') {
        int dir = (p->side == 1 ? -1 : 1), start = (p->side == 1 ? 6 : 1);
        if (dc == 0 && dr == dir && dest == '.') {}
        else if (dc == 0 && dr == 2 * dir && r == start && dest == '.' && p->b[idx(r + dir, c)] == '.') {}
        else if (abs(dc) == 1 && dr == dir && dest != '.') {}
        else return 0;
        if (tr == 0 || tr == 7) *promo = 'q';
        return 1;
    }
    if (t == 'n') return (abs(dr) == 2 && abs(dc) == 1) || (abs(dr) == 1 && abs(dc) == 2);
    if (t == 'k') return abs(dr) <= 1 && abs(dc) <= 1 && (dr || dc);
    int diagonal = (abs(dr) == abs(dc) && dr != 0), straight = ((dr == 0) != (dc == 0));
    if (t == 'b' && !diagonal) return 0;
    if (t == 'r' && !straight) return 0;
    if (t == 'q' && !(diagonal || straight)) return 0;
    return pathClear(p, from, to);
}

void makeMove(Position *p, Move m) {
    char pc = p->b[m.from];
    p->b[m.from] = '.';
    if (m.promo) {
        pc = (p->side == 1 ? (char)toupper((unsigned char)m.promo) : (char)tolower((unsigned char)m.promo));
    }
    p->b[m.to] = pc;
    p->side = -p->side;
}

int inCheck(Position *p, int side) {
    int kingSq = -1;
    char kingChar = (side == 1 ? 'K' : 'k');
    for (int i = 0; i < 64; i++) {
        if (p->b[i] == kingChar) {
            kingSq = i;
            break;
        }
    }
    if (kingSq == -1) return 1;
    Position temp = *p;
    temp.side = -side;
    char promo = 0;
    for (int f = 0; f < 64; f++) {
        if (color(p->b[f]) == -side) {
            if (pseudoLegal(&temp, f, kingSq, &promo)) return 1;
        }
    }
    return 0;
}

int legalMove(Position *p, int from, int to, Move *out) {
    char promo = 0;
    if (!pseudoLegal(p, from, to, &promo)) return 0;
    Position q = *p;
    Move m = {from, to, promo};
    makeMove(&q, m);
    if (inCheck(&q, p->side)) return 0;
    if (out) *out = m;
    return 1;
}

/* ========================================================================= */
/*                   1. LINKED LIST: Dynamic Legal Move List                 */
/* ========================================================================= */

typedef struct MoveNode {
    Move move;
    struct MoveNode *next;
} MoveNode;

typedef struct {
    MoveNode *head;
    int count;
} MoveList;

void moveListInit(MoveList *list) {
    list->head = NULL;
    list->count = 0;
}

void moveListPush(MoveList *list, Move m) {
    MoveNode *node = (MoveNode*)malloc(sizeof(MoveNode));
    if (!node) {
        fprintf(stderr, "Fatal error: MoveList allocation failed.\n");
        exit(1);
    }
    node->move = m;
    node->next = list->head;
    list->head = node;
    list->count++;
}

void moveListFree(MoveList *list) {
    MoveNode *curr = list->head;
    while (curr) {
        MoveNode *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->count = 0;
}

/* Populates a dynamic Linked List with all legal moves for the current position */
int genMoves(Position *p, MoveList *list) {
    moveListInit(list);
    for (int f = 0; f < 64; f++) {
        if (color(p->b[f]) == p->side) {
            for (int t = 0; t < 64; t++) {
                Move m;
                if (legalMove(p, f, t, &m)) {
                    moveListPush(list, m);
                }
            }
        }
    }
    return list->count;
}

/* ========================================================================= */
/*                    2. STACK (LIFO): Move Undo History                     */
/* ========================================================================= */

typedef struct HistoryNode {
    Position pos;
    Move move;
    struct HistoryNode *next;
} HistoryNode;

typedef struct {
    HistoryNode *top;
    int size;
} MoveStack;

void stackInit(MoveStack *s) {
    s->top = NULL;
    s->size = 0;
}

void stackPush(MoveStack *s, Position p, Move m) {
    HistoryNode *node = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (!node) {
        fprintf(stderr, "Fatal error: MoveStack allocation failed.\n");
        exit(1);
    }
    node->pos = p;
    node->move = m;
    node->next = s->top;
    s->top = node;
    s->size++;
}

int stackPop(MoveStack *s, Position *outPos, Move *outMove) {
    if (!s->top) return 0;
    HistoryNode *temp = s->top;
    if (outPos) *outPos = temp->pos;
    if (outMove) *outMove = temp->move;
    s->top = temp->next;
    free(temp);
    s->size--;
    return 1;
}

void stackFree(MoveStack *s) {
    Position dummyP;
    Move dummyM;
    while (stackPop(s, &dummyP, &dummyM));
}

/* ========================================================================= */
/*                   3. QUEUE (FIFO): Candidate Move Queue                   */
/* ========================================================================= */

typedef struct QueueNode {
    Move move;
    int score;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int count;
} MoveQueue;

void queueInit(MoveQueue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

void queueEnqueue(MoveQueue *q, Move m, int score) {
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) {
        fprintf(stderr, "Fatal error: MoveQueue allocation failed.\n");
        exit(1);
    }
    node->move = m;
    node->score = score;
    node->next = NULL;
    if (!q->rear) {
        q->front = node;
        q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->count++;
}

int queueDequeue(MoveQueue *q, Move *outMove, int *outScore) {
    if (!q->front) return 0;
    QueueNode *temp = q->front;
    if (outMove) *outMove = temp->move;
    if (outScore) *outScore = temp->score;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free(temp);
    q->count--;
    return 1;
}

void queueFree(MoveQueue *q) {
    Move dummyM;
    int dummyS;
    while (queueDequeue(q, &dummyM, &dummyS));
}

/* ========================================================================= */
/*                          4. EVALUATION FUNCTION                           */
/* ========================================================================= */

int evaluate(Position *p) {
    int score = 0;
    for (int i = 0; i < 64; i++) {
        char pc = p->b[i];
        if (pc == '.') continue;
        int v = value(pc), r = i / 8, c = i % 8;
        /* Small central-square bonus for active pieces. */
        if (tolower((unsigned char)pc) != 'k') v += (3 - abs(3 - r)) + (3 - abs(3 - c));
        score += color(pc) * v;
    }
    return score;
}

/* ========================================================================= */
/*                        5. N-ARY GAME TREE DATA STRUCTURE                  */
/* ========================================================================= */

/*
 * GameTreeNode represents a state in the N-ary Game Tree.
 * - pos: The chess board position at this node.
 * - move: The move made by the parent to reach this state.
 * - score: Evaluated minimax value of this state.
 * - childCount: Number of branch/child nodes explored from this state.
 * - children: Dynamic array of child pointers representing branches.
 */
typedef struct GameTreeNode {
    Position pos;
    Move move;
    int score;
    int childCount;
    struct GameTreeNode **children;
} GameTreeNode;

typedef struct {
    int totalNodes;
} TreeMetrics;

/*
 * Frees the entire Game Tree using post-order traversal to prevent memory leaks.
 */
void freeGameTree(GameTreeNode *node) {
    if (!node) return;
    for (int i = 0; i < node->childCount; i++) {
        freeGameTree(node->children[i]);
    }
    if (node->children) {
        free(node->children);
    }
    free(node);
}

/*
 * Recursively builds the Game Tree up to the specified depth.
 * Uses the dynamic Linked List for move generation and Alpha-Beta for pruning.
 */
GameTreeNode* buildGameTree(Position *p, Move moveFromParent, int depth, int alpha, int beta, int ply, TreeMetrics *metrics) {
    GameTreeNode *node = (GameTreeNode*)malloc(sizeof(GameTreeNode));
    if (!node) {
        fprintf(stderr, "Fatal error: GameTreeNode allocation failed.\n");
        exit(1);
    }
    if (metrics) metrics->totalNodes++;

    node->pos = *p;
    node->move = moveFromParent;
    node->childCount = 0;
    node->children = NULL;

    MoveList moveList;
    int n = genMoves(p, &moveList);

    /* Leaf / terminal node: Checkmate or Stalemate */
    if (n == 0) {
        if (inCheck(p, p->side)) {
            node->score = (p->side == 1) ? (-100000 + ply) : (100000 - ply);
        } else {
            node->score = 0; /* Stalemate */
        }
        moveListFree(&moveList);
        return node;
    }

    /* Leaf node: Maximum search depth reached, evaluate board position */
    if (depth == 0) {
        node->score = evaluate(p);
        moveListFree(&moveList);
        return node;
    }

    /* Allocate pointers for child branches */
    node->children = (GameTreeNode**)malloc(sizeof(GameTreeNode*) * n);
    if (!node->children) {
        fprintf(stderr, "Fatal error: memory allocation failed.\n");
        exit(1);
    }

    MoveNode *curr = moveList.head;
    if (p->side == 1) {
        /* Maximizing player (White) */
        int best = -INF;
        while (curr) {
            Position q = *p;
            makeMove(&q, curr->move);
            GameTreeNode *child = buildGameTree(&q, curr->move, depth - 1, alpha, beta, ply + 1, metrics);
            node->children[node->childCount++] = child;

            if (child->score > best) best = child->score;
            if (best > alpha) alpha = best;
            if (beta <= alpha) {
                /* Alpha-Beta cutoff: prune remaining sibling branches in the tree */
                break;
            }
            curr = curr->next;
        }
        node->score = best;
    } else {
        /* Minimizing player (Black) */
        int best = INF;
        while (curr) {
            Position q = *p;
            makeMove(&q, curr->move);
            GameTreeNode *child = buildGameTree(&q, curr->move, depth - 1, alpha, beta, ply + 1, metrics);
            node->children[node->childCount++] = child;

            if (child->score < best) best = child->score;
            if (best < beta) beta = best;
            if (beta <= alpha) {
                /* Alpha-Beta cutoff: prune remaining sibling branches in the tree */
                break;
            }
            curr = curr->next;
        }
        node->score = best;
    }

    moveListFree(&moveList);
    return node;
}

void squareName(int sq, char out[3]);

/*
 * Chooses the best move by constructing the Game Tree, and then
 * enqueues candidate moves into a FIFO Queue for evaluation and display.
 */
Move chooseMove(Position *p, int depth) {
    Move nullMove = {-1, -1, 0};
    Move bestMove = {-1, -1, 0};
    TreeMetrics metrics = {0};

    /* Build the Game Tree starting from current position as Root */
    GameTreeNode *root = buildGameTree(p, nullMove, depth, -INF, INF, 0, &metrics);

    if (!root || root->childCount == 0) {
        if (root) freeGameTree(root);
        return bestMove;
    }

    /* 1. Select the child node corresponding to the optimal minimax score */
    int best = (p->side == 1 ? -INF : INF);
    for (int i = 0; i < root->childCount; i++) {
        GameTreeNode *child = root->children[i];
        if ((p->side == 1 && child->score > best) || (p->side == -1 && child->score < best)) {
            best = child->score;
            bestMove = child->move;
        }
    }

    /* Fallback to first available child move if none improved */
    if (bestMove.from < 0 && root->childCount > 0) {
        bestMove = root->children[0]->move;
    }

    printf("[Game Tree] Analyzed %d nodes across depth %d (Root evaluation: %d, Root branches: %d)\n",
           metrics.totalNodes, depth, root->score, root->childCount);

    /* 2. QUEUE DEMO: Enqueue candidate root moves into a FIFO Queue */
    MoveQueue q;
    queueInit(&q);
    for (int i = 0; i < root->childCount; i++) {
        queueEnqueue(&q, root->children[i]->move, root->children[i]->score);
    }

    printf("[Move Queue (FIFO)] Enqueued %d candidate branches. Top evaluated responses:\n", q.count);
    Move m;
    int sc;
    int shown = 0;
    while (queueDequeue(&q, &m, &sc)) {
        if (shown < 3) {
            char s1[3], s2[3];
            squareName(m.from, s1);
            squareName(m.to, s2);
            printf("  -> Candidate %d: %s %s (score: %+d)%s\n",
                   shown + 1, s1, s2, sc, (m.from == bestMove.from && m.to == bestMove.to) ? " [BEST]" : "");
            shown++;
        }
    }
    queueFree(&q);

    /* Free game tree after move selection to avoid memory leaks */
    freeGameTree(root);
    return bestMove;
}

int parseSquare(const char *s) {
    if (strlen(s) != 2 || s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[1] > '8') return -1;
    return (8 - (s[1] - '0')) * 8 + (s[0] - 'a');
}

void squareName(int sq, char out[3]) {
    out[0] = (char)('a' + sq % 8);
    out[1] = (char)('8' - sq / 8);
    out[2] = '\0';
}

void printBoard(Position *p) {
    printf("\n    a b c d e f g h\n");
    printf("  +-----------------+\n");
    for (int r = 0; r < 8; r++) {
        printf("%d | ", 8 - r);
        for (int c = 0; c < 8; c++) {
            printf("%c ", p->b[r * 8 + c]);
        }
        printf("| %d\n", 8 - r);
    }
    printf("  +-----------------+\n");
    printf("    a b c d e f g h\n\n");
}

int main(void) {
    Position p;
    memcpy(p.b, startBoard, 64);
    p.side = 1;
    int depth = 3;
    char line[100], a[16], b[16], cmd[16];

    /* Initialize Stack for Move History / Undo */
    MoveStack history;
    stackInit(&history);

    puts("CHESS BOT IN C (Powered by Tree, Stack, Queue, & Linked List)");
    puts("You are White. Enter moves like: e2 e4");
    puts("Commands: 'undo' to revert last turn, 'quit' to exit.");
    printf("Choose AI depth (1-4, recommended 2 or 3): ");
    fflush(stdout);

    if (fgets(line, sizeof(line), stdin) && sscanf(line, "%d", &depth) == 1) {
        if (depth < 1) depth = 1;
        if (depth > 4) depth = 4;
    } else {
        depth = 3;
    }

    while (1) {
        printBoard(&p);

        /* Generate legal moves using dynamic Linked List */
        MoveList legalList;
        int n = genMoves(&p, &legalList);
        moveListFree(&legalList);

        if (n == 0) {
            puts(inCheck(&p, p.side) ? (p.side == 1 ? "Checkmate. Black wins." : "Checkmate. White wins.") : "Stalemate. Draw.");
            break;
        }

        if (p.side == 1) {
            printf("Your move (e.g. e2 e4, undo, or quit) [Stack: %d moves]: ", history.size);
            fflush(stdout);
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%15s", cmd) != 1) continue;

            if (strcmp(cmd, "quit") == 0) break;

            /* STACK DEMO: Undo functionality using LIFO pop */
            if (strcmp(cmd, "undo") == 0) {
                Position prevPos;
                Move prevMove;
                if (!stackPop(&history, &prevPos, &prevMove)) {
                    puts("[Stack] No moves to undo!");
                    continue;
                }
                p = prevPos;
                /* If computer made a reply, pop player's previous move too so it's White's turn again */
                if (history.size > 0 && p.side == -1) {
                    if (stackPop(&history, &prevPos, &prevMove)) {
                        p = prevPos;
                    }
                }
                puts("[Stack (LIFO)] Undid move! Board restored to previous state.");
                continue;
            }

            if (sscanf(line, "%15s %15s", a, b) != 2) {
                puts("Enter source and destination, e.g. e2 e4 (or 'undo').");
                continue;
            }
            int from = parseSquare(a), to = parseSquare(b);
            Move m;
            if (from < 0 || to < 0 || !legalMove(&p, from, to, &m)) {
                puts("Invalid move. Try again.");
                continue;
            }

            /* STACK: Push state before making move */
            stackPush(&history, p, m);
            makeMove(&p, m);
        } else {
            puts("Computer is thinking...");
            Move m = chooseMove(&p, depth);
            if (m.from < 0) {
                puts("No legal move.");
                break;
            }
            char s1[3], s2[3];
            squareName(m.from, s1);
            squareName(m.to, s2);
            printf("Computer plays: %s %s\n", s1, s2);

            /* STACK: Push state before making computer move */
            stackPush(&history, p, m);
            makeMove(&p, m);
            if (inCheck(&p, p.side)) puts("Check!");
        }
    }

    /* Free MoveStack history */
    stackFree(&history);
    puts("Game ended.");
    return 0;
}
