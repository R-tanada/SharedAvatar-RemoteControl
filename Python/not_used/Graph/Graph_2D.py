# -----------------------------------------------------------------------
# Author:   Takumi Nishimura (haptics lab)
# Created:  2021/12/23
# Summary:  Graph Manager
# -----------------------------------------------------------------------

import matplotlib.pyplot as plt

class Graph_2D:
    def __init__(self,n=1) -> None:
        self.x_list = []
        self.y_list = [[] for _ in range(n)]
    def make_list(self,x,*y):
        self.x_list.append(x)
        for i in range(len(y)):
                self.y_list[i].append(y[i])
        return self.x_list,self.y_list
    def soloy_graph(self):
        fig,ax = plt.subplots()
        for i in range(len(self.y_list)):
            ax.plot(self.x_list,self.y_list[i],label=('data%s'%(i+1)))
        ax.legend()
        plt.show()