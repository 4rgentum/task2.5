#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// Структура для представления вершины графа
typedef struct Vertex {
  char *name;
  struct Edge* adjacencyList; // Указатель на список смежности
  int visited; // Флаг посещения вершины
  int rating; // Суммарная оценка уровня отношений
} Vertex;

// Структура для представления ребра графа
typedef struct Edge {
  int rating;
  struct Vertex* destination; // Указатель на вершину, к которой ведет ребро
  struct Edge* next; // Указатель на следующее ребро в списке смежности
} Edge;

typedef struct Graph {
  int numVertices;
  struct Vertex* vertices;
} Graph;

char* readline() {
  char buffer[81] = {0};
  char *my_string = NULL;
  int length = 0;
  int element = 0;
  do {
    int flag = 0;
    element = scanf("%80[^\n]%n", buffer, &flag);
    if (element < 0 || flag == EOF) {
      if (!my_string) {
        return NULL;
      } else if (flag == EOF) {
        break;
      }
    } else if (element > 0 && flag != EOF) {
      int chunk_len = strlen(buffer);
      int str_len = length + chunk_len;
      char* temp = realloc(my_string, str_len + 1);
      if (!temp) {
        free(my_string);
        return NULL;
      }
      my_string = temp;
      memcpy(my_string + length, buffer, chunk_len);
      length = str_len;
      my_string[str_len] = '\0';
    } else {
      scanf("%*[^\n]");
      scanf("%*c");
    }
  } while (element > 0);
    if (my_string) {
      char* temp = realloc(my_string, length + 1);
      if (!temp) {
        free(my_string);
        return NULL;
      }
      my_string = temp;
      my_string[length] = '\0';
    } else {
      my_string = calloc(1, sizeof(char));
    }
  return my_string;
}

int get_unsigned_int() {
  int number;
  int flag = scanf("%u", &number);
  if (flag == -1) {
    return -1;
  } 
  while ((flag != 1 || number < 0) && flag != EOF) {
    printf("\nInvalid Input");
    printf("\nEnter Correct Unsigned Integer Number:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    flag = scanf("%u", &number);
    if (flag == -1) {
      return -1;
    }
  }
  scanf("%*[^\n]");
  scanf("%*c");
  return number;
}

int get_int() {
  int number;
  int flag = scanf("%d", &number);
  if (flag == -1) {
    return 0;
  } 
  while ((flag != 1 || number < 0) && flag != EOF) {
    printf("\nInvalid Input");
    printf("\nEnter Correct Integer Number:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    flag = scanf("%d", &number);
    if (flag == -1) {
      return 0;
    }
  }
  scanf("%*[^\n]");
  scanf("%*c");
  return number;
}

// Функция для создания новой вершины графа
struct Vertex* createVertex(const char* name) {
  struct Vertex* vertex = (struct Vertex*)malloc(sizeof(struct Vertex));
  strncpy(vertex->name, name, strlen(name));
  vertex->adjacencyList = NULL;
  return vertex;
}

// Функция для создания нового ребра графа
struct Edge* createEdge(struct Vertex* destination, int rating) {
  struct Edge* edge = (struct Edge*)malloc(sizeof(struct Edge));
  edge->destination = destination;
  edge->rating = rating;
  edge->next = NULL;
  return edge;
}

struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = 0;
    graph->vertices = NULL;
    return graph;


int addVertex(Graph* graph, const char* name) {
  if (graph == NULL) {
    return 1;
  }
  struct Vertex* newVertex = createVertex(name);
  graph->vertices = realloc(graph->vertices, (graph->numVertices + 1) * sizeof(struct Vertex));
  graph->vertices[graph->numVertices] = *newVertex;
  graph->numVertices++;
  return 0;
}

int addEdge(struct Graph* graph, const char* sourceName, const char* destinationName, int rating) {
  if (graph == NULL) {
    return 2;
  }
  struct Vertex* sourceVertex = NULL;
  struct Vertex* destinationVertex = NULL;
  // Найти вершины источника и назначения в графе
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, sourceName) == 0) {
      sourceVertex = &(graph->vertices[i]);
    } else if (strcmp(graph->vertices[i].name, destinationName) == 0) {
      destinationVertex = &(graph->vertices[i]);
    }
  }
  // Если вершины не найдены, вывести ошибку и выйти из функции
  if (sourceVertex == NULL || destinationVertex == NULL) {
    return 1;
  }
  // Создать новое ребро и добавить его к списку смежности вершины источника
  struct Edge* newEdge = createEdge(destinationVertex, rating);
  if (sourceVertex->adjacencyList == NULL) {
    sourceVertex->adjacencyList = newEdge;
  } else {
    struct Edge* current = sourceVertex->adjacencyList;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newEdge;
  }
  return 0;
}

void deleteEdge(struct Vertex* vertex, struct Vertex* destination) {
  struct Edge* current = vertex->adjacencyList;
  struct Edge* prev = NULL;
  while (current != NULL) {
    if (current->destination == destination) {
      if (prev == NULL) {
        vertex->adjacencyList = current->next;
      } else {
        prev->next = current->next;
      }
      free(current);
      return;
    }
    prev = current;
    current = current->next;
  }
}

int deleteVertex(struct Graph* graph, const char* name) {
  if (graph == NULL) {
    return 2;
  }
  int vertexIndex = -1;
  // Найти индекс вершины в графе
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, name) == 0) {
      vertexIndex = i;
      break;
    }
  }
  // Если вершина не найдена, вывести ошибку и выйти из функции
  if (vertexIndex == -1) {
    return 1;
  }
  struct Vertex* vertex = &(graph->vertices[vertexIndex]);
  // Удалить все ребра, ведущие к этой вершине
  for (int i = 0; i < graph->numVertices; i++) {
    if (i != vertexIndex) {
      deleteEdge(&(graph->vertices[i]), vertex);
    }
  }
  // Освободить память, выделенную для имени вершины
  free(vertex->name);
  // Сдвинуть остальные вершины в массиве, чтобы удалить вершину
  for (int i = vertexIndex; i < graph->numVertices - 1; i++) {
    graph->vertices[i] = graph->vertices[i + 1];
  }
  // Уменьшить количество вершин
  graph->numVertices--;
  // Перераспределить память для массива вершин
  graph->vertices = (struct Vertex*)realloc(graph->vertices, graph->numVertices * sizeof(struct Vertex));
  return 0;
}

int deleteOnlyEdge(struct Graph* graph, const char* sourceName, const char* destinationName) {
  if (graph == NULL) {
    return 2;
  }
  struct Vertex* sourceVertex = NULL;
  struct Vertex* destinationVertex = NULL;
  // Найти вершины источника и назначения в графе
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, sourceName) == 0) {
      sourceVertex = &(graph->vertices[i]);
    } else if (strcmp(graph->vertices[i].name, destinationName) == 0) {
      destinationVertex = &(graph->vertices[i]);
    }
  }
  // Если вершины не найдены, вывести ошибку и выйти из функции
  if (sourceVertex == NULL || destinationVertex == NULL) {
    return 1;
  }
  // Удалить ребро из списка смежности вершины источника
  struct Edge* current = sourceVertex->adjacencyList;
  struct Edge* prev = NULL;
  while (current != NULL) {
    if (current->destination == destinationVertex) {
      if (prev == NULL) {
        sourceVertex->adjacencyList = current->next;
      } else {
        prev->next = current->next;
      }
      free(current);
      return 0;
    }
    prev = current;
    current = current->next;
  }
  return 1;
}

int updateVertexData(struct Graph* graph, const char* vertexName, const char* newData) {
  if (graph == NULL) {
    return 2;
  }
  struct Vertex* vertex = NULL;
  // Найти вершину в графе
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, vertexName) == 0) {
      vertex = &(graph->vertices[i]);
      break;
    }
  }
  // Если вершина не найдена, вывести ошибку и выйти из функции
  if (vertex == NULL) {
    return 1;
  }
  // Обновить данные вершины
  free(vertex->name);
  vertex->name = strdup(newData);
  return 0;
}

int updateEdgeData(struct Graph* graph, const char* sourceName, const char* destinationName, int newRating) {
  if (graph == NULL) {
    return 2;
  }
  struct Vertex* sourceVertex = NULL;
  struct Vertex* destinationVertex = NULL;
  // Найти вершины источника и назначения в графе
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, sourceName) == 0) {
      sourceVertex = &(graph->vertices[i]);
    } else if (strcmp(graph->vertices[i].name, destinationName) == 0) {
      destinationVertex = &(graph->vertices[i]);
    }
  }
  // Если вершины не найдены, вывести ошибку и выйти из функции
  if (sourceVertex == NULL || destinationVertex == NULL) {
    return 1;
  }
  // Найти ребро в списке смежности вершины источника и обновить данные
  struct Edge* current = sourceVertex->adjacencyList;
  while (current != NULL) {
    if (current->destination == destinationVertex) {
      current->rating = newRating;
      return 0;
    }
    current = current->next;
  }
  return 1;
}

int printGraph(Graph* graph) {
  if (graph == NULL) {
    return 1;
  }
  for (int i = 0; i < graph->numVertices; i++) {
    struct Vertex* vertex = &(graph->vertices[i]);
    printf("Vertex: %s\n", vertex->name);
    struct Edge* current = vertex->adjacencyList;
    while (current != NULL) {
      printf("-> %s (Rating: %d)\n", current->destination->name, current->rating);
      current = current->next;
    }
    printf("\n");
  }
  return 0;
}

void clearGraph(Graph* graph) {
  if (graph == NULL)
    return;
  for (int i = 0; i < graph->numVertices; i++) {
    struct Vertex* vertex = &(graph->vertices[i]);
    struct Edge* current = vertex->adjacencyList;
    while (current != NULL) {
      struct Edge* next = current->next;
      free(current);
      current = next;
    }
    free(vertex->name);
  }
  free(graph->vertices);
  graph->vertices = NULL;
  graph->numVertices = 0;
  free(graph);
}

void dfs(struct Vertex* vertex) {
  printf("%s ", vertex->name);
  struct Edge* current = vertex->adjacencyList;
  while (current != NULL) {
    if (current->rating >= 0) {
      dfs(current->destination);
    }
    current = current->next;
  }
}

int findPotentialFriends(Graph* graph, const char* personName) {
  if (graph == NULL) {
    return 2;
  }
  struct Vertex* startVertex = NULL;
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, personName) == 0) {
      startVertex = &(graph->vertices[i]);
      break;
    }
  }
  if (startVertex == NULL) {
    return 1;
  }
  printf("Potential friends for %s: ", startVertex->name);
  dfs(startVertex);
  printf("\n");
  return 0;
}

// Вспомогательная функция для поиска вершины по имени
struct Vertex* findVertexByName(Graph* graph, const char* name) {
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, name) == 0) {
      return &(graph->vertices[i]);
    }
  }
  return NULL; // Вершина не найдена
}

void dijkstra(Graph* graph, const char* startName, const char* endName) {
  if (graph == NULL) {
    printf("No Graph?");
    return;
  }
  // Находим начальную и конечную вершины
  struct Vertex* startVertex = findVertexByName(graph, startName);
  struct Vertex* endVertex = findVertexByName(graph, endName);
  if (startVertex == NULL || endVertex == NULL) {
    printf("Одна или обе указанные вершины не существуют.\n");
    return;
  }
  // Создаем массив для хранения расстояний от начальной вершины до остальных
  int* distances = (int*)malloc(graph->numVertices * sizeof(int));
  // Инициализируем расстояния для всех вершин как бесконечность, кроме начальной
  for (int i = 0; i < graph->numVertices; i++) {
    distances[i] = INT_MAX;
  }
  distances[startVertex - graph->vertices] = 0; // Расстояние от начальной вершины до самой себя равно 0
  // Создаем массив для хранения предыдущих вершин на пути к каждой вершине
  struct Vertex** previousVertices = (struct Vertex**)malloc(graph->numVertices * sizeof(struct Vertex*));
  // Инициализируем предыдущие вершины как NULL
  for (int i = 0; i < graph->numVertices; i++) {
    previousVertices[i] = NULL;
  }
  // Итеративно обновляем расстояния до всех соседних вершин
  struct Vertex* currentVertex = startVertex;
  while (currentVertex != NULL) {
    currentVertex->visited = 1; // Помечаем текущую вершину как посещенную
    struct Edge* currentEdge = currentVertex->adjacencyList;
    while (currentEdge != NULL) {
      struct Vertex* neighbor = currentEdge->destination;
      if (!neighbor->visited && currentEdge->rating > 0) { // Проверяем только положительные ребра
        int distance = distances[currentVertex - graph->vertices] + currentEdge->rating;
        if (distance < distances[neighbor - graph->vertices]) {
          distances[neighbor - graph->vertices] = distance;
          previousVertices[neighbor - graph->vertices] = currentVertex;
        }
      }
      currentEdge = currentEdge->next;
    }
    // Находим следующую непосещенную вершину с наименьшим расстоянием
    int minDistance = INT_MAX;
    struct Vertex* nextVertex = NULL;
    for (int i = 0; i < graph->numVertices; i++) {
      if (!graph->vertices[i].visited && distances[i] < minDistance) {
        minDistance = distances[i];
        nextVertex = &(graph->vertices[i]);
      }
    }
    currentVertex = nextVertex;
  }
  // Проверяем, достижима ли конечная вершина
  if (previousVertices[endVertex - graph->vertices] == NULL) {
    printf("Конечная вершина недостижима из начальной вершины.\n");
    free(distances);
    free(previousVertices);
    return;
  }
  // Восстанавливаем кратчайший путь
  int pathLength = 0;
  struct Vertex* path[graph->numVertices];
  currentVertex = endVertex;
  while (currentVertex != startVertex) {
    path[pathLength++] = currentVertex;
    currentVertex = previousVertices[currentVertex - graph->vertices];
  }
  path[pathLength++] = startVertex;
  // Выводим кратчайший путь в обратном порядке
  printf("Кратчайший путь между %s и %s с исключительно положительным отношением:\n", startName, endName);
  for (int i = pathLength - 1; i >= 0; i--) {
    printf("%s ", path[i]->name);
  }
  printf("\n");
  free(distances);
  free(previousVertices);
}

void floydWarshall(Graph* graph) {
  int numVertices = graph->numVertices;
  // Инициализация матрицы расстояний
  int** distances = (int**)malloc(numVertices * sizeof(int*));
  for (int i = 0; i < numVertices; i++) {
    distances[i] = (int*)malloc(numVertices * sizeof(int));
    for (int j = 0; j < numVertices; j++) {
      if (i == j) {
        distances[i][j] = 0;
      } else {
        distances[i][j] = INT_MAX;
      }
    }
  }
  // Заполнение матрицы расстояний
  for (int i = 0; i < numVertices; i++) {
    struct Vertex* currentVertex = &(graph->vertices[i]);
    struct Edge* currentEdge = currentVertex->adjacencyList;
    while (currentEdge != NULL) {
      int destinationIndex = currentEdge->destination - graph->vertices;
      distances[i][destinationIndex] = currentEdge->rating;
      currentEdge = currentEdge->next;
    }
  }
  // Выполнение алгоритма Флойда-Уоршалла
  for (int k = 0; k < numVertices; k++) {
    for (int i = 0; i < numVertices; i++) {
      for (int j = 0; j < numVertices; j++) {
        if (distances[i][k] != INT_MAX && distances[k][j] != INT_MAX) {
          int newDistance = distances[i][k] + distances[k][j];
          if (newDistance < distances[i][j]) {
            distances[i][j] = newDistance;
          }
        }
      }
    }
  }
  // Обновление суммарной оценки уровня отношений для каждой вершины
  for (int i = 0; i < numVertices; i++) {
    int ratingSum = 0;
    for (int j = 0; j < numVertices; j++) {
      if (distances[i][j] != INT_MAX) {
        ratingSum += distances[i][j];
      }
    }
    graph->vertices[i].rating = ratingSum;
  }
  // Освобождение памяти
  for (int i = 0; i < numVertices; i++) {
    free(distances[i]);
  }
  free(distances);
}

struct Vertex* findPotentialFriend(Graph* graph, const char* personName) {
  struct Vertex* person = NULL;
  for (int i = 0; i < graph->numVertices; i++) {
    if (strcmp(graph->vertices[i].name, personName) == 0) {
      person = &(graph->vertices[i]);
      break;
    }
  }
  if (person == NULL) {
    printf("Указанный человек не существует в графе.\n");
    return NULL;
  }
  struct Vertex* potentialFriend = NULL;
  int maxRating = 0;
  for (int i = 0; i < graph->numVertices; i++) {
    if (graph->vertices[i].rating > maxRating) {
      // Проверяем, что вершина не является указанным человеком и имеет положительную оценку отношений
      if (strcmp(graph->vertices[i].name, personName) != 0 && graph->vertices[i].rating > 0) {
        potentialFriend = &(graph->vertices[i]);
        maxRating = graph->vertices[i].rating;
      }
    }
  }
  if (potentialFriend == NULL) {
    printf("У указанного человека нет потенциальных знакомых.\n");
    return NULL;
  }
  return potentialFriend;
}

int D_AddVertex(Graph *graph) {
  int rc = 0;
  const char *errmsgs[] = {"Ok", "No Graph?"};
  printf("Input Name:\n");
  char *name = readline();
  if (name == NULL) {
    free(name);
    return 0;
  }
  rc = addVertex(graph, name);
  printf("%s\n", errmsgs[rc]);
  free(name);
  return 1;
}

int D_AddEdge(Graph *graph) {
  int rc = 0;
  const char *errmsgs[] = {"Ok", "No Elemes", "No Graph?"};
  printf("Input Source Name:\n");
  char *source = readline();
  if (source == NULL) {
    free(source);
    return 0;
  }
  printf("Input Destin Name:\n");
  char *dest = readline();
  if (dest == NULL) {
    free(source);
    free(dest);
    return 0;
  }
  printf("Input Rating:\n");
  int rating = get_int();
  if (rating == 0) {
    free(source);
    free(dest);
    return 0;
  }
  rc = addEdge(graph, source, dest, rating);
  printf("%s\n", errmsgs[rc]);
  free(source);
  free(dest);
  return 1;
}

int D_DeleteOnlyEdge(Graph *graph) {
  int rc = 0;
  const char* errmsgs[]={"Ok", "No Elem?", "No Graph?"};
  printf("Input Source Name:\n");
  char *source = readline();
  if (source == NULL) {
    free(source);
    return 0;
  }
  printf("Input Destin Name:\n");
  char *dest = readline();
  if (dest == NULL) {
    free(source);
    free(dest);
    return 0;
  }
  rc = deleteOnlyEdge(graph, source, dest);
  printf("%s\n", errmsgs[rc]);
  free(source);
  free(dest);
  return 1;
}

int D_DelteVertex(Graph *graph) {
  int rc = 0;
  const char* errmsgs[]={"Ok", "No Elem?", "No Graph?"};
  printf("Input Name:\n");
  char *name = readline();
  if (name == NULL) {
    free(name);
    return 0;
  }
  rc = deleteVertex(graph, name);
  printf("%s\n", errmsgs[rc]);
  free(name);
  return 1;
}

int D_UpdateVertex(Graph *graph) {
  int rc = 0;
  const char* errmsgs[]={"Ok", "No Elem?", "No Graph?"};
  printf("Input Source Name:\n");
  char *source = readline();
  if (source == NULL) {
    free(source);
    return 0;
  }
  printf("Input New Name:\n");
  char *data = readline();
  if (data == NULL) {
    free(source);
    free(data);
    return 0;
  }
  rc = updateVertexData(graph, source, data);
  printf("%s\n", errmsgs[rc]);
  free(source);
    free(data);
    return 1;
}

int D_UpdateEdge(Graph *graph) {
  int rc = 0;
  const char *errmsgs[] = {"Ok", "No Elemes", "No Graph?"};
  printf("Input Source Name:\n");
  char *source = readline();
  if (source == NULL) {
    free(source);
    return 0;
  }
  printf("Input Destin Name:\n");
  char *dest = readline();
  if (dest == NULL) {
    free(source);
    free(dest);
    return 0;
  }
  printf("Input Rating:\n");
  int rating = get_int();
  if (rating == 0) {
    free(source);
    free(dest);
    return 0;
  }
  rc = updateEdgeData(graph, source, dest, rating);
  printf("%s\n", errmsgs[rc]);
  free(source);
  free(dest);
  return 1;
}

int D_Print(Graph *graph) {
  int rc = 0;
  const char* errmsgs[]={"Graph", "No Graph?"};
  rc = printGraph(graph);
  printf("%s \n",errmsgs[rc]);
  return 1;
}

int D_DFS(Graph *graph) {
  int rc = 0;
  const char* errmsgs[]={"Ok", "No Elem?", "No Graph?"};
  printf("Input Name:\n");
  char *name = readline();
  if (name == NULL) {
    free(name);
    return 0;
  }
  rc = findPotentialFriends(graph, name);
  printf("%s\n", errmsgs[rc]);
  free(name);
  return 1;
}

int D_Dijkstra(Graph *graph) {
  printf("Input Source Name:\n");
  char *source = readline();
  if (source == NULL) {
    free(source);
    return 0;
  }
  printf("Input Destin Name:\n");
  char *dest = readline();
  if (dest == NULL) {
    free(source);
    free(dest);
    return 0;
  }
  dijkstra(graph, source, dest);
  free(source);
  free(dest);
  return 1;
}

int D_FloydWarshall(Graph *graph) {

  printf("Input Name:\n");
  char *name = readline();
  if (name == NULL) {
    free(name);
    return 0;
  }
  findPotentialFriend(graph, name);
  free(name);
  return 1;
}

int dialog(const char* msgs[], int flag){
  char* errmsg = "";
  int rc;
  do {
    puts(errmsg);
    errmsg = "Invalid Input, Repeat\n";
    for (int i = 0; i < flag; i++) {
      puts(msgs[i]);
    }
    puts("Enter:\n");
    rc = get_int();
    printf("\n");
    
  } while (rc < 0 || rc >= flag);
  return rc;
}

int main() {
  Graph *graph = createGraph();
  const char* msgs[] = {"0. Quit", "1. Add Vertex", "2. Add Edge", "3. Delete Only Edge", "4. Delete Vertex", "5. Print Graph", "6. DFS", "7. Dijrstra", "8. Floyd Warshall", "9. Update Vertex", "10. Update Edge"};
  const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
  int (*fptr[])(Graph *) = {NULL, D_AddVertex, D_AddEdge, D_DeleteOnlyEdge, D_DelteVertex, D_Print, D_DFS, D_Dijkstra, D_FloydWarshall, D_UpdateVertex, D_UpdateEdge};
  int rc;
  while (rc = dialog(msgs, NMsgs)) {
    if (!fptr[rc](graph)){
      break;
    }
  }
  printf("End\n");
  clearGraph(graph);
  return 0;
}

/*int main() {
    // Пример создания графа
    Graph graph;
    graph.numVertices = 6;
    graph.vertices = (struct Vertex*)malloc(graph.numVertices * sizeof(struct Vertex));

    // Инициализация вершин графа
    graph.vertices[0].name = "Алиса";
    graph.vertices[0].adjacencyList = NULL;
    graph.vertices[0].rating = 0;

    graph.vertices[1].name = "Боб";
    graph.vertices[1].adjacencyList = NULL;
    graph.vertices[1].rating = 0;

    graph.vertices[2].name = "Карл";
    graph.vertices[2].adjacencyList = NULL;
    graph.vertices[2].rating = 0;

    graph.vertices[3].name = "Джейн";
    graph.vertices[3].adjacencyList = NULL;
    graph.vertices[3].rating = 0;

    graph.vertices[4].name = "Мария";
    graph.vertices[4].adjacencyList = NULL;
    graph.vertices[4].rating = 0;

    graph.vertices[5].name = "Петр";
    graph.vertices[5].adjacencyList = NULL;
    graph.vertices[5].rating = 0;

    // Добавление ребер и их оценок для заданной цепочки знакомств
    // Пример: Алиса -> Боб -> Карл -> Мария -> Петр
    struct Edge* aliceBob = (struct Edge*)malloc(sizeof(struct Edge));
    aliceBob->rating = 5;
    aliceBob->destination = &(graph.vertices[1]);
    aliceBob->next = NULL;
    graph.vertices[0].adjacencyList = aliceBob;

    struct Edge* bobCarl = (struct Edge*)malloc(sizeof(struct Edge));
    bobCarl->rating = 3;
    bobCarl->destination = &(graph.vertices[2]);
    bobCarl->next = NULL;
    graph.vertices[1].adjacencyList = bobCarl;

    struct Edge* carlMaria = (struct Edge*)malloc(sizeof(struct Edge));
    carlMaria->rating = 4;
    carlMaria->destination = &(graph.vertices[4]);
    carlMaria->next = NULL;
    graph.vertices[2].adjacencyList = carlMaria;

    struct Edge* mariaPetr = (struct Edge*)malloc(sizeof(struct Edge));
    mariaPetr->rating = 2;
    mariaPetr->destination = &(graph.vertices[5]);
    mariaPetr->next = NULL;
    graph.vertices[4].adjacencyList = mariaPetr;

    // Выполнение алгоритма Флойда-Уоршалла
    floydWarshall(&graph);

    // Поиск потенциального знакомого с наибольшей суммарной оценкой уровня отношений для Алисы
    struct Vertex* potentialFriend = findPotentialFriend(&graph, "Алиса");

    if (potentialFriend != NULL) {
        printf("Потенциальный знакомый для Алисы: %s\n", potentialFriend->name);
    }

    // Освобождение памяти
    for (int i = 0; i < graph.numVertices; i++) {
        struct Edge* currentEdge = graph.vertices[i].adjacencyList;
        while (currentEdge != NULL) {
            struct Edge* nextEdge = currentEdge->next;
            free(currentEdge);
            currentEdge = nextEdge;
        }
    }
    free(graph.vertices);

    return 0;
}*/
