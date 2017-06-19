# coding=utf-8
# author=veficos


import random
import time
import math

random.seed(time.time())

fitness_chromosome = [1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0] + [0]*32


def rand_bool():
    return random.randint(0, 1) == 1


def rand_float():
    return random.random()


def rand_clamped():
    return rand_float() - rand_float()


def rand_int(x, y):
    return random.randint(x, y)


class Genome(object):
    """ 基因组 """

    def __init__(self, chromosome_length: int, gene_length: int):

        # 染色体
        self.chromosome = [rand_int(0, 1) for _ in range(chromosome_length)]

        # 染色体长度
        self.chromosome_length = chromosome_length

        # 基因片段长度
        self.gene_length = gene_length

        # 适应性
        self.fitness = 0

    def decode(self):
        """ 解码基因 """

        # 迷宫方向
        directions = []

        for i in range(0, len(self.chromosome), self.gene_length):
            # 基因片段
            gene = self.chromosome[i:i+self.gene_length]

            # 根据基因片段解码出方向，00：北，01：南，02：东，03：西
            directions.append(int(''.join([str(x) for x in gene]), 2))

        return directions

    @staticmethod
    def crossover(crossover_rate, father, mother):
        """ 根据交配率对两条基因组进行交配，并生成两个新生儿 """

        assert (father.chromosome_length == mother.chromosome_length)
        assert (father.gene_length == mother.gene_length)

        if (rand_float() > crossover_rate) or (father == mother):
            return father, mother

        chromosome_length = father.chromosome_length
        gene_length = father.gene_length
        crossover_point = rand_int(0, chromosome_length - 1)

        baby1 = Genome(chromosome_length, gene_length)
        baby2 = Genome(chromosome_length, gene_length)
        baby1.chromosome = mother.chromosome[0: crossover_point]
        baby2.chromosome = father.chromosome[0: crossover_point]
        baby1.chromosome += father.chromosome[crossover_point: chromosome_length]
        baby2.chromosome += mother.chromosome[crossover_point: chromosome_length]

        return baby1, baby2

    @staticmethod
    def mutate(mutation_rate, genome):
        """ 基因变异 """

        for i in range(len(genome.chromosome)):
            if rand_float() < mutation_rate:
                genome.chromosome[i] = abs(1 - genome.chromosome[i])

    def __str__(self):
        return 'chromosome: ' + ''.join([str(x) for x in self.chromosome]) + ', ' + \
            'fitness: ' + str(self.fitness)


class Maze(object):
    """ 迷宫 """
    def __init__(self):
        self.map = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1],
            [8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1],
            [1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1],
            [1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5],
            [1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
        ]

        self.map_height = len(self.map)
        self.map_width = len(self.map[0])

        self.start_x = 2
        self.start_y = 0
        self.end_x = 7
        self.end_y = 14

    def test_route(self, tmp_maze, path):
        pos_x = self.start_x
        pos_y = self.start_y

        for direction in path:
            if direction == 0x00:
                # 向北移动
                if ((pos_x - 1) < 0) or (self.map[pos_x - 1][pos_y] == 1):
                    break
                else:
                    pos_x -= 1
            elif direction == 0x01:
                # 向南移动
                if ((pos_x + 1) >= self.map_height) or (self.map[pos_x + 1][pos_y] == 1):
                    break
                else:
                    pos_x += 1
            elif direction == 0x02:
                # 向东移动
                if ((pos_y + 1) >= self.map_width) or (self.map[pos_x][pos_y + 1] == 1):
                    break
                else:
                    pos_y += 1
            elif direction == 0x03:
                # 向西移动
                if ((pos_y - 1) < 0) or (self.map[pos_x][pos_y - 1] == 0):
                    break
                else:
                    pos_y -= 1
            else:
                assert(True is False)

            tmp_maze.map[pos_x][pos_y] = 2


        #diff_x = (pos_x - self.end_x) ** 2
        #diff_y = (pos_y - self.end_y) ** 2
        #return 1 / (math.sqrt(diff_x + diff_y) + 1)
        diff_x = abs(pos_x - self.end_x)
        diff_y = abs(pos_y - self.end_y)
        return 1 / (diff_x + diff_y + 1)

    def print_map(self):
        element = {
            0: '-',     # 可走路径
            1: '+',     # 墙壁
            5: '$',     # 起始位置
            8: '^',     # 重点位置
            2: '>',     # 已走过
        }

        for line in self.map:
            [print(element[x], end='') for x in line]
            print('\n')


class Evolution(object):
    """ 遗传器 """

    def __init__(self, crossover_rate: float, mutation_rate: float,
                 genomes_length: int, chromo_length: int, gene_length: int):

        # 基因组群
        self.genomes = []

        # 基因组群的大小
        self.genomes_length = genomes_length

        # 杂交率
        self.crossover_rate = crossover_rate

        # 变异率
        self.mutation_rate = mutation_rate

        # 一条染色体有多长
        self.chromo_length = chromo_length

        # 每个基因含有多少bits
        self.gene_length = gene_length

        # 当前最优的基因
        self.fittest_genome = None

        # 最优的基因分数
        self.best_fitness_score = 0

        # 已尝试过基因总分数
        self.total_fitness_score = 0

        # 是否还在交配中
        self.is_run = False

        # 已经历经几代
        self.generation = 0

        self.maze = Maze()

    def create_start_genomes(self):
        """ 创建初始基因组群 """

        self.genomes = [Genome(self.chromo_length, self.gene_length) for _ in range(self.genomes_length)]

        # g = Genome(self.chromo_length, self.gene_length)
        # g.chromosome = fitness_chromosome
        # self.genomes.append(g)

        self.fittest_genome = None
        self.best_fitness_score = 0
        self.total_fitness_score = 0
        self.generation = 0

    def run(self):
        self.create_start_genomes()
        self.is_run = True

    def epoch(self):
        self.update_fitness_scores()

        new_babies = 0
        baby_genomes = []

        while new_babies < self.genomes_length:
            father = self.roulette_wheel_selection()
            mother = self.roulette_wheel_selection()

            baby1, baby2 = Genome.crossover(self.crossover_rate, father, mother)
            Genome.mutate(self.mutation_rate, baby1)
            Genome.mutate(self.mutation_rate, baby2)

            baby_genomes.append(baby1)
            baby_genomes.append(baby2)
            new_babies += 2

        self.genomes = baby_genomes
        self.generation += 1

    def update_fitness_scores(self):
        """ 
        用新的适应性分数来更新基因组原有的适应性分数
        并计算群体的最高适应性分数和适应性分数最高的那个成员
        """

        self.fittest_genome = None
        self.best_fitness_score = 0
        self.total_fitness_score = 0

        for genome in self.genomes:
            test_path_maze = Maze()
            directions = genome.decode()
            genome.fitness = self.maze.test_route(test_path_maze, directions)

            self.total_fitness_score += genome.fitness
            self.maze = test_path_maze

            if genome.fitness == 1:
                # 找到路径
                self.is_run = False
                break

            if genome.fitness > self.best_fitness_score:
                # 挑选优质的基因
                self.best_fitness_score = genome.fitness
                self.fittest_genome = genome

    def roulette_wheel_selection(self):
        """ 赌盘选择法挑选基因 """
        slice = rand_float() * self.total_fitness_score
        total = 0.0

        for genome in self.genomes:
            total += genome.fitness
            if total > slice:
                return genome
        return self.genomes[0]

    def roulette_wheel_selection2(self):
        """ 赌盘选择法挑选基因 """
        slice = rand_float() * self.total_fitness_score
        total = 0.0

        genomes = self.genomes[:]
        result = [self.fittest_genome]

        for genome in genomes:
            if genome == self.fittest_genome:
                continue

            total += genome.fitness
            if total > slice:
                result.append(genome)
                break
        else:
            result.append(self.genomes[rand_int(0, len(genomes)-1)])

        return result[0], result[1]

    @property
    def is_running(self):
        return self.is_run


CROSSOVER_RATE = 0.7
MUTATION_RATE = 0.001
GENOMES_SIZE = 140
CHROMO_LENGTH = 52
GENE_LENGTH = 2

evo = Evolution(CROSSOVER_RATE, MUTATION_RATE, GENOMES_SIZE, CHROMO_LENGTH, GENE_LENGTH)

evo.run()
while evo.is_running:
    evo.epoch()

print(evo.generation)
evo.maze.print_map()
print('\n\n\n\n')
