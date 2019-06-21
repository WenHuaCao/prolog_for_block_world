#include <iostream>
#include <vector>
#include <time.h>
using namespace std;
const int SIZE = 4;
//const int SIZE = 5;
//const int SIZE = 6;
//const int SIZE = 7;
//const int SIZE = 8;
//const int SIZE = 9;
class curDom
{
public:
	int val;					//the value of the element
	bool visit;					//decide whether the element is visited(valued)
	bool domain[SIZE + 1];		//the domain of the element
	curDom()					//initialization
	{
		val = 0;
		visit = false;
		for(int i = 0;i < SIZE + 1;i ++)
			domain[i] = true;
	}
};
typedef curDom element[SIZE][SIZE];//get new name for convenience
element result;					   //the result of the problem
class Index						   //get the position
{
public:
	int x;
	int y;
	Index()
	{
		x = 0;
		y = 0;
	}
	Index(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};
//get the relation of two numbers 1:left < right up < down  2: left > right up > down
class Operation					
{
public:
	Index left;		//the position which is left/up from the operator
	Index right;	//the position which is right/down form the operator
	int num;		//the operator
	Operation(Index _left, Index _right, int _num)
	{
		left.x = _left.x, left.y = _left.y;
		right.x = _right.x, right.y = _right.y;
		num = _num;
	}
};
//decide whether the position parameter's domain is empty
//temp:current matrix parament:the position of the element
bool is_empty(element temp, Index parameter) 
{
	int x = parameter.x;
	int y = parameter.y;
	for(int i = 1;i <= SIZE;i ++)
	{
		if(temp[x][y].domain[i])
		{
			return false;
		}
	}
	return true;
}
//delete the condition-unmatched value in domain of the left element and right element
///temp:current matrix 	parament:one condition
bool change_compare(element temp, Operation parameter)
{
	int left_x = parameter.left.x;
	int left_y = parameter.left.y;
	int right_x = parameter.right.x;
	int right_y = parameter.right.y;
	int num = parameter.num;
	//have a visited and a not visited, delete domain of the not visit
	if((temp[left_x][left_y].visit && !temp[right_x][right_y].visit) || (!temp[left_x][left_y].visit && temp[right_x][right_y].visit) )
	{
		switch(num)
		{
			case 1: 	//<
				if(temp[left_x][left_y].visit)
					{
						int val = temp[left_x][left_y].val;
						for(int i = 1;i <= val;i ++)
							temp[right_x][right_y].domain[i] = false;
						if(is_empty(temp, Index(right_x, right_y)))
							return false;	//decide whether domain of a element in condition after the removing
					}
				else 
					{
						int val = temp[right_x][right_y].val;
						for(int i = val;i <= SIZE;i ++)
							temp[left_x][left_y].domain[i] = false;
						if(is_empty(temp, Index(left_x, left_y)))
								return false;	//decide whether domain of a element in condition after the removing
					}
				break;
			case 2:		//>
				if(temp[left_x][left_y].visit)
					{
						int val = temp[left_x][left_y].val;
						for(int i = val;i <= SIZE;i ++)
							temp[right_x][right_y].domain[i] = false;
						if(is_empty(temp, Index(right_x, right_y)))
							return false;	//decide whether domain of a element in condition after the removing
					}
				else 
					{
						int val = temp[right_x][right_y].val;
						for(int i = 1;i <= val;i ++)
							temp[left_x][left_y].domain[i] = false;
						if(is_empty(temp, Index(left_x, left_y)))
								return false;	//decide whether domain of a element in condition after the removing
					}
				break;
		}
	}
	//two not visit, delete the domains of two not visit
	else if(!temp[left_x][left_y].visit && !temp[right_x][right_y].visit)
	{
		switch(num)
		{
			int i, j;
			case 1:	//<
				for(i = 1;i <= SIZE;i ++)
				{
					if(temp[left_x][left_y].domain[i])
					{
						for(j = i + 1;j <= SIZE;j ++)
						{
							if(temp[right_x][right_y].domain[j])
								break;
						}
						if(j == SIZE + 1)
							temp[left_x][left_y].domain[i] = false;
					}
				}
				for(i = 1;i <= SIZE;i ++)
				{
					if(temp[right_x][right_y].domain[i])
					{
						for(j = 1;j < i;j ++)
						{
							if(temp[left_x][left_y].domain[j])
								break;
						}
						if(j == i)
							temp[right_x][right_y].domain[i] = false;
					}
				}
				if(is_empty(temp, Index(left_x, left_y)) || is_empty(temp, Index(right_x, right_y)))
					return false;	//decide whether domain of a element in condition after the removing
				break;
			case 2:	//>
				for(i = 1;i <= SIZE;i ++)
				{
					if(temp[left_x][left_y].domain[i])
					{
						for(j = 1;j < i;j ++)
						{
							if(temp[right_x][right_y].domain[j])
								break;
						}
						if(j == i)
							temp[left_x][left_y].domain[i] = false;
					}
				}
				for(i = 1;i <= SIZE;i ++)
				{
					if(temp[right_x][right_y].domain[i])
					{
						for(j = i + 1;j <= SIZE;j ++)
						{
							if(temp[left_x][left_y].domain[j])
								break;
						}
						if(j == SIZE + 1)
							temp[right_x][right_y].domain[i] = false;
					}
				}
				if(is_empty(temp, Index(left_x, left_y)) || is_empty(temp, Index(right_x, right_y)))
					return false;	//decide whether domain of a element in condition after the removing
				break;
		}
	}
	
	return true;
}
//Ensure diff(col)
bool change_column(element temp, Index parameter)
{
	int x = parameter.x;
	int y = parameter.y;
	int val = temp[x][y].val;
	for(int i = 0;i < SIZE;i ++)
	{
		if(!temp[i][y].visit)
		{
			temp[i][y].domain[val] = false;
			if(is_empty(temp, Index(i, y)))
				return false;
		}
	}
	return true;
}
//Ensure diff(row)
bool change_row(element temp, Index parameter)
{
	int x = parameter.x;
	int y = parameter.y;
	int val = temp[x][y].val;
	for(int i = 0;i < SIZE;i ++)
	{
		if(!temp[x][i].visit)
		{
			temp[x][i].domain[val] = false;
			if(is_empty(temp, Index(x, i)))
				return false;
		}
	}
	return true;
}
//temp: current matrix  parameter:the position of valued parameter
//val:value, conditions: the set of conditions
bool GAC_Enforce(element temp, Index parameter, int val, vector<Operation> &conditions)
{
	int x = parameter.x;
	int y = parameter.y;
	temp[x][y].visit = true;
	temp[x][y].val = val;	//get the visit
	bool flag1 = true;
	//check every condition
	int len = conditions.size();
	for(int i = 0;i < len;i ++)
	{
		flag1 &= change_compare(temp, conditions[i]);
	}
	bool flag2 = change_column(temp, Index(x, y));
	bool flag3 = change_row(temp, Index(x, y));
	return flag1 && flag2 && flag3;
}
void copy_matrix(element temp, element copied)
{
	for(int i = 0;i < SIZE;i ++)
		for(int j = 0;j < SIZE;j ++)
		{
			temp[i][j].val = copied[i][j].val;
			temp[i][j].visit = copied[i][j].visit;
			for(int k = 0;k <= SIZE;k ++)
				temp[i][j].domain[k] = copied[i][j].domain[k];
		}
}
void output(element temp)
{
	for(int i = 0;i < SIZE;i ++)
	{
		for(int j = 0;j < SIZE;j ++)
			cout << temp[i][j].val << " ";
	 	cout << endl;
	}
}
bool GAC(element other, int x, int y, vector<Operation> &conditions)
{
	//get the result
	if(x == SIZE  && y == 0 && other[x - 1][SIZE - 1].visit)
	{
		copy_matrix(result, other);
		return true;
	}
	//if the position is visied(initialization of the matrix)
	else if(other[x][y].visit)
	{
		int next_x = y + 1 == SIZE ? x + 1 : x;
		int next_y = y + 1 == SIZE ? 0 : y + 1;
		return GAC(other, next_x, next_y, conditions);
	}
	else
	{
		//check the domain of the position
		for(int i = 1;i <= SIZE;i ++)
		{
			if(other[x][y].domain[i] == false)
				continue;
			else
			{
				element temp;
				copy_matrix(temp, other);
				int val = i;
				//if GAC_Enforce is false, there is a empty domain
				if(GAC_Enforce(temp, Index(x,y), val, conditions) == false)
					continue;
				else
				{
					int next_x = y + 1== SIZE ? x + 1 : x;
					int next_y = y + 1== SIZE ? 0 : y + 1;
					bool flag = GAC(temp, next_x, next_y, conditions);
					if(flag)
						return flag;
				}
			}
		}
	}
	return false;
}
void input_1(element temp)
{
	vector<Operation> conditions;
	conditions.push_back(Operation(Index(0, 2), Index(0,3), 2));
	conditions.push_back(Operation(Index(0, 1), Index(1,1), 1));
	conditions.push_back(Operation(Index(1, 2), Index(2,2), 1));
	conditions.push_back(Operation(Index(3, 0), Index(3,1), 2));
	conditions.push_back(Operation(Index(3, 1), Index(3,2), 2));
	GAC_Enforce(temp, Index(0, 2), 3, conditions);
	GAC(result, 0, 0, conditions);
}
void input_2(element temp)
{
	vector<Operation> conditions;
	conditions.push_back(Operation(Index(0, 0), Index(0, 1), 2));
	conditions.push_back(Operation(Index(0, 0), Index(1, 0), 1));
	conditions.push_back(Operation(Index(1, 1), Index(1, 2), 1));
	conditions.push_back(Operation(Index(1, 2), Index(1, 3), 1));
	conditions.push_back(Operation(Index(1, 3), Index(1, 4), 1));
	conditions.push_back(Operation(Index(2, 1), Index(2, 2), 1));
	conditions.push_back(Operation(Index(4, 0), Index(4, 1), 2));
	GAC_Enforce(temp, Index(4, 4), 4, conditions);
	GAC(temp, 0, 0, conditions);

}
void input_3(element temp)
{
	vector<Operation> conditions;
	conditions.push_back(Operation(Index(0, 0), Index(0, 1), 2));
	conditions.push_back(Operation(Index(1, 3), Index(1, 4), 2));
	conditions.push_back(Operation(Index(2, 0), Index(2, 1), 1));
	conditions.push_back(Operation(Index(1, 1), Index(2, 1), 2));
	conditions.push_back(Operation(Index(1, 5), Index(2, 5), 1));
	conditions.push_back(Operation(Index(3, 2), Index(3, 3), 2));
	conditions.push_back(Operation(Index(3, 3), Index(3, 4), 2));
	conditions.push_back(Operation(Index(5, 3), Index(5, 4), 1));
	conditions.push_back(Operation(Index(5, 4), Index(5, 5), 1));
	GAC_Enforce(temp, Index(0, 4), 2, conditions);
	GAC_Enforce(temp, Index(0, 5), 6, conditions);
	GAC_Enforce(temp, Index(1, 5), 3, conditions);
	GAC_Enforce(temp, Index(2, 0), 3, conditions);
	GAC_Enforce(temp, Index(3, 2), 4, conditions);
	GAC(temp, 0, 0, conditions);
}
void input_4(element temp)
{
	vector<Operation> conditions;
	conditions.push_back(Operation(Index(0, 0), Index(0, 1), 1));
	conditions.push_back(Operation(Index(0, 1), Index(0, 2), 2));
	conditions.push_back(Operation(Index(0, 5), Index(0, 6), 2));
	conditions.push_back(Operation(Index(1, 4), Index(1, 5), 2));
	conditions.push_back(Operation(Index(2, 0), Index(2, 1), 1));
	conditions.push_back(Operation(Index(2, 1), Index(2, 2), 1));
	conditions.push_back(Operation(Index(1, 2), Index(2, 2), 2));
	conditions.push_back(Operation(Index(2, 5), Index(2, 6), 2));
	conditions.push_back(Operation(Index(3, 1), Index(3, 2), 1));
	conditions.push_back(Operation(Index(2, 4), Index(3, 4), 1));
	conditions.push_back(Operation(Index(3, 4), Index(3, 5), 2));
	conditions.push_back(Operation(Index(3, 1), Index(4, 1), 2));
	conditions.push_back(Operation(Index(4, 1), Index(4, 2), 2));
	conditions.push_back(Operation(Index(3, 3), Index(4, 3), 1));
	conditions.push_back(Operation(Index(5, 0), Index(5, 1), 1));
	conditions.push_back(Operation(Index(4, 2), Index(5, 2), 2));
	conditions.push_back(Operation(Index(5, 4), Index(6, 4), 1));
	conditions.push_back(Operation(Index(5, 5), Index(6, 5), 2));
	conditions.push_back(Operation(Index(6, 5), Index(6, 6), 2));
	GAC_Enforce(temp, Index(0, 6), 6, conditions);
	GAC_Enforce(temp, Index(3, 6), 2, conditions);
	GAC_Enforce(temp, Index(5, 1), 5, conditions);
	GAC(temp, 0, 0, conditions);
}
void input_5(element temp)
{
	vector<Operation> conditions;
	conditions.push_back(Operation(Index(0, 1), Index(0, 2), 2));
	conditions.push_back(Operation(Index(0, 2), Index(0, 3), 2));
	conditions.push_back(Operation(Index(0, 4), Index(0, 5), 1));
	conditions.push_back(Operation(Index(0, 5), Index(0, 6), 1));
	conditions.push_back(Operation(Index(0, 6), Index(0, 7), 1));
	conditions.push_back(Operation(Index(1, 0), Index(1, 1), 1));
	conditions.push_back(Operation(Index(1, 5), Index(1, 6), 1));
	conditions.push_back(Operation(Index(1, 2), Index(2, 2), 1));
	conditions.push_back(Operation(Index(2, 2), Index(2, 3), 1));
	conditions.push_back(Operation(Index(1, 3), Index(2, 3), 1));
	conditions.push_back(Operation(Index(1, 5), Index(2, 5), 2));
	conditions.push_back(Operation(Index(1, 6), Index(2, 6), 2));
	conditions.push_back(Operation(Index(3, 0), Index(3, 1), 2));
	conditions.push_back(Operation(Index(3, 1), Index(3, 2), 2));
	conditions.push_back(Operation(Index(2, 3), Index(3, 3), 2));
	conditions.push_back(Operation(Index(4, 0), Index(4, 1), 2));
	conditions.push_back(Operation(Index(3, 3), Index(4, 3), 2));
	conditions.push_back(Operation(Index(4, 6), Index(4, 7), 1));
	conditions.push_back(Operation(Index(3, 7), Index(4, 7), 2));
	conditions.push_back(Operation(Index(5, 4), Index(5, 5), 1));
	conditions.push_back(Operation(Index(4, 5), Index(5, 5), 1));
	conditions.push_back(Operation(Index(5, 5), Index(5, 6), 2));
	conditions.push_back(Operation(Index(4, 7), Index(5, 7), 1));
	conditions.push_back(Operation(Index(6, 2), Index(6, 3), 2));
	conditions.push_back(Operation(Index(5, 4), Index(6, 4), 2));
	GAC_Enforce(temp, Index(1, 4), 6, conditions);
	GAC_Enforce(temp, Index(1, 6), 7, conditions);
	GAC_Enforce(temp, Index(2, 3), 4, conditions);
	GAC_Enforce(temp, Index(4, 7), 6, conditions);
	GAC_Enforce(temp, Index(5, 5), 4, conditions);
	GAC_Enforce(temp, Index(6, 7), 3, conditions);
	GAC(temp, 0, 0, conditions);
}
void input_6(element temp)
{
	vector<Operation> conditions;
	conditions.push_back(Operation(Index(0, 0), Index(0, 1), 1));
	conditions.push_back(Operation(Index(0, 2), Index(0, 3), 2));
	conditions.push_back(Operation(Index(1, 3), Index(1, 4), 1));
	conditions.push_back(Operation(Index(1, 6), Index(1, 7), 1));
	conditions.push_back(Operation(Index(2, 0), Index(2, 1), 2));
	conditions.push_back(Operation(Index(2, 2), Index(2, 3), 1));
	conditions.push_back(Operation(Index(1, 6), Index(2, 6), 2));
	conditions.push_back(Operation(Index(3, 2), Index(3, 3), 2));
	conditions.push_back(Operation(Index(2, 3), Index(3, 3), 1));
	conditions.push_back(Operation(Index(3, 4), Index(3, 5), 2));
	conditions.push_back(Operation(Index(3, 5), Index(3, 6), 1));
	conditions.push_back(Operation(Index(3, 7), Index(3, 8), 2));
	conditions.push_back(Operation(Index(4, 0), Index(4, 1), 1));
	conditions.push_back(Operation(Index(3, 1), Index(4, 1), 2));
	conditions.push_back(Operation(Index(3, 5), Index(4, 5), 2));
	conditions.push_back(Operation(Index(5, 1), Index(5, 2), 1));
	conditions.push_back(Operation(Index(4, 4), Index(5, 4), 2));
	conditions.push_back(Operation(Index(5, 4), Index(5, 5), 1));
	conditions.push_back(Operation(Index(5, 6), Index(5, 7), 2));
	conditions.push_back(Operation(Index(4, 8), Index(5, 8), 2));
	conditions.push_back(Operation(Index(5, 1), Index(6, 1), 1));
	conditions.push_back(Operation(Index(6, 3), Index(6, 4), 1));
	conditions.push_back(Operation(Index(5, 6), Index(6, 6), 2));
	conditions.push_back(Operation(Index(5, 8), Index(6, 8), 2));
	conditions.push_back(Operation(Index(6, 7), Index(7, 7), 2));
	conditions.push_back(Operation(Index(7, 1), Index(8, 1), 1));
	conditions.push_back(Operation(Index(7, 2), Index(8, 2), 2));
	conditions.push_back(Operation(Index(7, 5), Index(8, 5), 1));
	conditions.push_back(Operation(Index(8, 5), Index(8, 6), 1));
	conditions.push_back(Operation(Index(7, 8), Index(8, 8), 2));
	GAC_Enforce(temp, Index(0, 3), 7, conditions);
	GAC_Enforce(temp, Index(0, 4), 3, conditions);
	GAC_Enforce(temp, Index(0, 5), 8, conditions);
	GAC_Enforce(temp, Index(0, 7), 5, conditions);
	GAC_Enforce(temp, Index(1, 2), 7, conditions);
	GAC_Enforce(temp, Index(1, 5), 2, conditions);
	GAC_Enforce(temp, Index(2, 5), 9, conditions);
	GAC_Enforce(temp, Index(3, 3), 4, conditions);
	GAC_Enforce(temp, Index(4, 2), 1, conditions);
	GAC_Enforce(temp, Index(4, 6), 6, conditions);
	GAC_Enforce(temp, Index(4, 7), 4, conditions);
	GAC_Enforce(temp, Index(5, 6), 2, conditions);
	GAC_Enforce(temp, Index(8, 8), 6, conditions);
	GAC(temp, 0, 0, conditions);

}
int main()
{
	clock_t start = clock();
	input_1(result);
	//input_2(result);
	//input_3(result);
	//input_4(result);
	//input_5(result);
	//input_6(result);
	clock_t end = clock();
	output(result);
	cout << "Running Time:" << (double)(end - start) / CLOCKS_PER_SEC << endl;
	return 0;
}
