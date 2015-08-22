#include <iostream>
#include <cstdlib>
#include <cstring>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

#ifdef _WIN32
#pragma comment (linker,"/subsystem:console")
#endif

#define BLACK   0		//空白
#define MINE    100	    //地雷
#define NOSWEEP 0	    //未扫过
#define SWEEP   1		//扫雷
#define FLAG    2		//标记

class WinMine
{
public:

	WinMine(int iRow, int iColumn);
	~WinMine();
public:
	void InitMine(int iMineMax);
	void SetColor();
	void StatisticsMine();
	void Map();
	bool OpenWhites(int x, int y, int status);
	void GameStart(int iMineMax);
	void GameOver();
	bool GoodOver();
private:
	int **m_ppMine;
	int **m_ppSweep;
	bool m_bMineflag;
	bool m_bSweepflag;
	int m_row;
	int m_column;
	int m_minenum;
};

int main(void)
{
	system("color 0d");
	while (true)
	{
		int level;
		WinMine *Mine;
		cout << "请输入游戏等级:" <<endl;
		cout << "1.初级" <<endl;
		cout << "2.中级" <<endl;
		cout << "3.高级" <<endl;
		cout << "4.退出" <<endl;
		cin >> level;
		if (level == 1)
		{
			Mine = new WinMine(9,9);
			Mine->GameStart(10);
		}
		else if (level == 2)
		{
			Mine = new WinMine(16,16);
			Mine->GameStart(40);
		}
		else if (level == 3)
		{
			Mine = new WinMine(16,30);
			Mine->GameStart(70);
		}
		else if (level == 4)
		{
			return 0;
		}
		else 
		{
			cout << "输入错误！" <<endl;
			continue;
		}
		delete Mine;
	}

	return 0;
}

WinMine::WinMine(int iRow, int iColumn)
{
	int i;
	
	//储雷状态
	m_ppMine = (int **) new int[iRow]; if (!m_ppMine) return;
	m_ppMine[0] = new int[iRow * iColumn]; if (!*m_ppMine) { delete[] m_ppMine; 

m_ppMine = NULL; return;}
	m_bMineflag = true;

	//扫雷状态
	m_ppSweep = (int **) new int[iRow]; if (!m_ppSweep) return;
	m_ppSweep[0] = new int[iRow * iColumn]; if (!*m_ppSweep) { delete[] m_ppSweep; 

m_ppSweep = NULL; return;}
	m_bSweepflag = true;

	m_row = iRow; m_column = iColumn;
	
	for (i = 1; i < iRow; i++)
	{
		m_ppMine[i] = m_ppMine[0] + i * iRow;
	}

	for (i = 1; i < iRow; i++)
	{
		m_ppSweep[i] = m_ppSweep[0] + i * iRow;
	}

	memset(m_ppSweep[0], 0, iRow * iColumn * sizeof(int));
	memset(m_ppMine[0], 0, iRow * iColumn * sizeof(int));
	
}

WinMine::~WinMine()
{
	if (m_bMineflag)
	{
		if (m_ppMine[0]) delete[] m_ppMine[0];
		if (m_ppMine) delete[] m_ppMine;
	}
	if (m_bSweepflag)
	{
		if (m_ppSweep[0]) delete[] m_ppSweep[0];
		if (m_ppSweep) delete[] m_ppSweep;
	}
}

//设置颜色
void WinMine::SetColor()
{
	system("color 0a");
}

//初始化雷数
void WinMine::InitMine(int iMineMax)	
{
	int x, y,num = 0;
	srand( (unsigned)time(NULL) );
	for (int i = 0; num != iMineMax; i++)
	{
		x = rand()%m_row;
		y = rand()%m_column;
		if (MINE != m_ppMine[x][y])
		{
			m_ppMine[x][y] = MINE;
			num++;
		}
	}
	m_minenum = num;

}

//统计雷数
void WinMine::StatisticsMine()
{
	int i, j;
	int n, m;
	int num = 0; //保存雷数

	//中间
	for ( i = 1; i < m_row - 1; i++)
	{
		for ( j = 1; j < m_column - 1; j++)
		{
			if ( m_ppMine[i][j] == BLACK)
			{
				for (n = i - 1; n <= i + 1; n++)
				{
					for (m = j - 1; m <= j + 1; m++)
					{
						if ( m_ppMine[n][m] == MINE )
							num++;

					}
				}
				m_ppMine[i][j] = num;
				num = 0;
			}

		}
	}

	//顶边
	for ( i = 1; i < m_column - 1; i++)
	{
		if (m_ppMine[0][i] == BLACK)
		{
			for (n = 0; n < 2; n++)
			{
				for (m = i - 1; m <= i + 1; m++)
				{
					if (m_ppMine[n][m] == MINE)
						num++;
				}
			}
			m_ppMine[0][i] = num;
			num = 0;
		}
	}

	//下边
	for ( i = 1; i < m_column - 1; i++)
	{
		if (m_ppMine[m_row - 1][i] == BLACK)
		{
			for (n = m_row - 2; n < m_row; n++)
			{
				for (m = i - 1; m <= i + 1; m++)
				{
					if (m_ppMine[n][m] == MINE)
						num++;
				}
			}
			m_ppMine[m_row - 1][i] = num;
			num = 0;
		}
	}

	//左边

	for ( i = 1; i < m_row - 1; i++ )
	{
		if (m_ppMine[i][0] == BLACK)
		{
			for (n = i - 1; n <= i + 1; n++)
			{
				for (m = 0; m < 2; m++)
					if (m_ppMine[n][m] == MINE)
						num++;	
			}

			m_ppMine[i][0] = num;
			num = 0;
		}
	}

	//右边
	for ( i = 1; i < m_row - 1; i++ )
	{
		if (m_ppMine[i][m_column - 1] == BLACK)
		{
			for (n = i - 1; n <= i + 1; n++)
			{
				for (m = m_column - 2; m < m_column; m++)
				{
					if (m_ppMine[n][m] == MINE)
						num++;
				}
			}
			m_ppMine[i][m_column - 1] = num;
			num = 0;
		}
	}

	//左上角
	if (m_ppMine[0][0] != MINE)
	{
		if (m_ppMine[0][1] == MINE)
			num++;
		if (m_ppMine[1][1] == MINE)
			num++;
		if (m_ppMine[1][0] == MINE)
			num++;
		m_ppMine[0][0] = num;
		num = 0;
	}


	//左下角

	if (m_ppMine[m_row - 1][0] != MINE)
	{
		if (m_ppMine[m_row - 2][0] == MINE)
			num++;
		if (m_ppMine[m_row - 2][1] == MINE)
			num++;
		if (m_ppMine[m_row - 1][1] == MINE)
			num++;
		m_ppMine[m_row - 1][0] = num;
		num = 0;
	}

	//右上角
	if (m_ppMine[0][m_column - 1] != MINE)
	{
		if (m_ppMine[1][m_column - 1] == MINE)
			num++;
		if (m_ppMine[1][m_column - 2] == MINE)
			num++;
		if (m_ppMine[0][m_column - 2] == MINE)
			num++;
		m_ppMine[0][m_column - 1] = num;
		num = 0;
	}

	//右下角
	if (m_ppMine[m_row - 1][m_column - 1] != MINE)
	{
		if (m_ppMine[m_row - 2][m_column - 1] == MINE)
			num++;
		if (m_ppMine[m_row - 2][m_column - 2] == MINE)
			num++;
		if (m_ppMine[m_row - 1][m_column - 2] == MINE)
			num++;
		m_ppMine[m_row - 1][m_column - 1] = num;
		num = 0;
	}
}

//展开空白
bool WinMine::OpenWhites(int row, int column, int status)
{
	if (row < 0 || row > (m_row - 1) || column < 0 || column > (m_column - 1))
		return true;
	if (status == SWEEP &&  m_ppMine[row][column] == MINE)
	{
		return false;
	}

	if (status == FLAG)
	{
		m_ppSweep[row][column] = FLAG;
		return true;
	}

	if (m_ppSweep[row][column] == NOSWEEP && m_ppMine[row][column] != MINE)
	{

		if (m_ppMine[row][column] > 0)
		{
			m_ppSweep[row][column] = SWEEP; 
			return true;
		}
		else
		{
			m_ppSweep[row][column] = SWEEP;
			OpenWhites(row-1, column, status);
			OpenWhites(row-1, column-1, status);
			OpenWhites(row-1, column+1, status);
			OpenWhites(row, column-1, status);
			OpenWhites(row, column+1, status);
			OpenWhites(row+1, column, status);
			OpenWhites(row+1, column-1, status);
			OpenWhites(row+1, column+1, status);
		}
	}
	return true;
}

//地图
void WinMine::Map()
{
	SetColor();
	int i, j;
	for ( i = 0; i < m_row; i++)
	{
		for (j = 0; j < m_column; j++)
		{
			if (m_ppSweep[i][j] == SWEEP)
			{
				if (m_ppMine[i][j] == 0)
				{
					cout << "□";
				}
				else if (m_ppMine[i][j] == 1)
				{
					cout << "①";
				}
				else if (m_ppMine[i][j] == 2)
				{
					cout << "②";
				}
				else if (m_ppMine[i][j] == 3)
				{
					cout << "③";
				}
				else if (m_ppMine[i][j] == 4)
				{
					cout << "④";
				}
				else if (m_ppMine[i][j] == 5)
				{
					cout << "⑤";
				}
				else if (m_ppMine[i][j] == 6)
				{
					cout << "⑥";
				}
				else if (m_ppMine[i][j] == 7)
				{
					cout << "⑦";
				}
				else if (m_ppMine[i][j] == 8)
				{
					cout << "⑧";
				}	

			}
			else if (m_ppSweep[i][j] == FLAG)
			{
				cout << "⊙";
			}
			else
				cout << "▇";
		}
		cout << endl;
	}
}

//游戏结束
void WinMine::GameOver()
{

	int i, j;
	for ( i = 0; i < m_row; i++)
	{
		for (j = 0; j < m_column; j++)
		{
			if (m_ppMine[i][j] == MINE)
				cout << "★";
			else 
				cout << "□";
			
		}
		cout << endl;
	}
}
//检查是否雷标记正确。
bool WinMine::GoodOver()
{
	int i, j;
	int num = 0;
	for (i = 0; i < m_row; i++)
	{
		for (j = 0; j < m_column; j++)
		{
			if (m_ppSweep[i][j] == FLAG && m_ppMine[i][j] == MINE)
				num++;

		}
	}
	if (num == m_minenum)
		return true;
	else
		return false;
}

//开始游戏
void WinMine::GameStart(int iMineMax)
{
	int x, y;
	int flag;
begin:
	memset(m_ppSweep[0], 0, m_row * m_column * sizeof(int));
	memset(m_ppMine[0], 0, m_row * m_column * sizeof(int));
	InitMine(iMineMax);
	StatisticsMine();

	while (true)
	{
#ifdef _WIN32
		system("cls");
#else
        system("clear");
#endif
		Map();
		cout << "请输入要扫的区域的坐标:" <<endl;
		cout << "请输入纵坐标:";cin>>x;
		cout << "请输入横坐标:";cin>>y;
		if (x <= 0 || x > m_row || y <= 0 || y > m_column)
		{
			cout <<"输入错误请重新输入" <<endl;
#ifdef _WIN32
			Sleep(1000);
#else   
            sleep(1);
#endif
			continue;
		}

		cout << "请输入是要做标记还是扫雷，扫雷请按1，标记请按2:" <<endl;
		cin >> flag;
		if ( false == OpenWhites(x-1, y-1, flag) )
		{
			int i;
#ifdef _WIN32
			system("cls");
#else
            system("clear");
#endif
			GameOver();
			cout << "游戏结束!" <<endl;
			cout << "继续游戏请按1，退出游戏请按0:"<<endl;
			cin >> i;
			if (i == 1)
				goto begin;
			else 
				goto end;
		}
		else
		{
			if (GoodOver() == true)
			{
				int i;
				cout << "扫雷成功！" <<endl;
				cout << "继续游戏请按1，退出游戏请按0:"<<endl;
				cin >> i;
				if (i == 1)
					goto begin;
				else 
					goto end;
			}
		}
	}
end:
	return;
}