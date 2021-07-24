# Feito por Matheus de Moraes Cavazotti (github.com/mcavazotti)

# USO:
# Para desenhar um arquivo de saída do Trabalho 1, faça:
# $ python3 draw.py < arquivo

import matplotlib
import matplotlib.pyplot as plt

# Definição de Ponto


class Point:
    def __init__(self, x: float = 0, y: float = 0) -> None:
        self.x = x
        self.y = y

    def __str__(self) -> str:
        return "({},{})".format(self.x, self.y)

    def __repr__(self) -> str:
        return self.__str__()



if __name__ == "__main__":
    print('matplotlib version: {}'.format(matplotlib.__version__))

    # Ler número de pontos
    numPoints = int(input())
    pointList = {}

    for p in range(numPoints):
        # Ler coordenadas dos pontos
        id, x, y = map(float, input().split())
        pointList[id] = Point(x, y)

    # Adicionar o primeiro ponto no fim da lista para fechar o polígono

    # Ler número de arestas
    numEdges = int(input())
    edgeList = []

    for t in range(numEdges):
        # Ler o índice dos pontos e ignorar o resto (não é necessário para plotar a figura)
        p1, p2, *rest = map(int, input().split())
        edgeList.append((p1, p2))

    print("Arestas: ", edgeList)

    # Inicializar figura
    fig, ax = plt.subplots()

    # Plotar triângulos em vermelho
    for e in edgeList:
        ax.plot([pointList[e[0]].x, pointList[e[1]].x], [pointList[e[0]].y, pointList[e[1]].y], color='#f00', lw=1)

    # Plotar pontos em preto
    ax.plot(list(map(lambda p: p.x, pointList.values())), list(
        map(lambda p: p.y, pointList.values())), 'o', color='#000', lw=3)

    # Ajustar a escala para não ter distorção
    plt.axis('scaled')

    # Mostrar figura
    plt.show()
