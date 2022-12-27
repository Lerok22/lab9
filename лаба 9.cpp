#include <iostream>
#include <list>
#include <stack>
#include <queue>
using namespace std;

struct node
{
    int vertex;
    struct node* next;
};
struct Graph
{
    int numVertices;
    struct node** adjLists;
};
struct node* createNode(int v)
{
    struct node* newNode = (node*)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}
struct Graph* createGraph(int vertices)
{
    struct Graph* graph = (Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = (node**)malloc(vertices * sizeof(struct node*));

    int i;
    for (i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;

    return graph;
}
void addEdge(struct Graph* graph, int src, int dest)
{
    // добавление вершины в список элемента
    struct node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

}
void printGraph(struct Graph* graph)
{
    for (int v = 0; v < graph->numVertices; v++)
    {
        struct node* temp = graph->adjLists[v];
        printf("\nВершина %d: ", v);
        while (temp)
        {
            printf("%d ", temp->vertex);
            temp = temp->next;
        }
    }
}
void toString(int** Matrix, int n) {
    for (int i = 0; i < n; i++) {
        cout << i << " : ";
        for (int j = 0; j < n; j++) {
            cout << Matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void DFS_M(int** Matrix, bool* visited, int n, int start, int dest)
{
    visited[start] = true;
    cout << start << " ";

    if (start == dest)
        for (int i = 0; i < n; i++)
            visited[i] = 1;

    for (int i = 0; i <= n; i++)
        if ((Matrix[start][i] != 0) && (!visited[i]))
            DFS_M(Matrix, visited, n, i, dest);
}

void DFS_S(int** Matrix, bool* visited, int n, struct Graph* graph, int start, int dest)
{
    struct node* spisok = graph->adjLists[start];
    struct node* temp = spisok;

    visited[start] = 1;
    printf("%d ", start);

    if (start == dest)
        for (int i = 0; i < n; i++)
            visited[i] = 1;

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (!visited[connectedVertex]) {
            DFS_S(Matrix, visited, n, graph, connectedVertex, dest);
        }
        temp = temp->next;
    }
}

int main()
{
    setlocale(0, "");
    srand(time(0));
    int n;
    printf_s("Введите кол-во вершин: \n");
    scanf_s("%d", &n);

    struct Graph* graph = createGraph(n);
    int** Matrix = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        Matrix[i] = (int*)malloc(sizeof(int*) * n);
        for (int j = 0; j < n; j++)
            Matrix[i][j] = 0; // создание матрицы смежности
    }

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (0 < rand() % 2) {
                Matrix[i][j] = Matrix[j][i] = 1;
                addEdge(graph, i, j);
                addEdge(graph, j, i);
            }

    bool* visited = (bool*)malloc(n * sizeof(bool));

    printf_s("Матрица смeжности: \n");
    toString(Matrix, n);

    printf_s("\nСписок смежности: ");
    printGraph(graph);

    int start, end;
    printf_s("\n\nВведите стартовую вершину: "); scanf_s("%d", &start);

    printf_s("\nПуть(DFS_матрица): ");
    for (end = 0; end < n; end++) {
        printf_s("\n");
        for (int i = 0; i < n; i++)
            visited[i] = 0;
        DFS_M(Matrix, visited, n, start, end);
    }

    printf_s("\nПуть(DFS_список): ");
    for (end = 0; end < n; end++) {
        printf_s("\n");
        for (int i = 0; i < n; i++)
            visited[i] = 0;
        DFS_S(Matrix, visited, n, graph, start, end);
    }

    int* dist = (int*)malloc(n * sizeof(int));
    int* pred = (int*)malloc(n * sizeof(int));
    queue<int> queue, q;

    // BFS для матрицы смежности
    printf_s("\nКратчайший Путь(BFS_матрица):");

    for (int i = 0; i < n; i++)
        visited[i] = 0;

    for (end = 0; end < n; end++) {
        int dest1 = end;
        printf_s("\n");

        for (int i = 0; i < n; i++) {
            dist[i] = INT_MAX;
            pred[i] = -1;
        }

        visited[start] = true;
        dist[start] = 0;
        queue.push(start);

        while (!queue.empty()) {
            int t = queue.front();
            queue.pop();

            for (int i = 0; i < n; i++) {
                if (Matrix[t][i] == 1 && (!visited[i])) {
                    visited[i] = true;
                    queue.push(i);
                    dist[i] = dist[t]++;
                    pred[i] = t;

                    if (i == dest1) {
                        printf("%d ", dest1);
                        while (pred[dest1] != -1) {
                            printf("%d ", pred[dest1]);
                            dest1 = pred[dest1];
                        }
                        break;
                    }

                }
            }
        }

        while (queue.size() != 0)
            queue.pop();

        for (int i = 0; i < n; i++)
            visited[i] = 0;
    }
    // для списка смежности
    printf_s("\nКратчайший Путь(BFS_список): ");

    for (int i = 0; i < n; i++)
        visited[i] = 0;

    for (end = 0; end < n; end++) {
        int dest2 = end;
        printf_s("\n");

        for (int i = 0; i < n; i++) {
            dist[i] = INT_MAX;
            pred[i] = -1;
        }
        visited[start] = 1;
        dist[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int currentVertex = q.front();
            q.pop();

            struct node* temp = graph->adjLists[currentVertex];
            while (temp != NULL) {
                int adjVertex = temp->vertex;
                if (!visited[adjVertex]) {
                    visited[adjVertex] = 1;
                    q.push(adjVertex);
                    dist[adjVertex] = dist[currentVertex]++;
                    pred[adjVertex] = currentVertex;

                    if (adjVertex == dest2) {
                        printf("%d ", dest2);
                        while (pred[dest2] != -1) {
                            printf("%d ", pred[dest2]);
                            dest2 = pred[dest2];
                        }
                        break;
                    }
                }
                temp = temp->next;
            }
        }
        while (queue.size() != 0)
            q.pop();

        for (int i = 0; i < n; i++)
            visited[i] = 0;
    }
    free(dist);
    free(pred);

    for (int i = 0; i < n; i++)
        free(Matrix[i]);
    free(Matrix);
    free(visited);

    return 0;
}