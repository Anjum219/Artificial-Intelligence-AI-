#include<bits/stdc++.h>

using namespace std;

int k;
int heuristic_choice;

void set_heuristic_choice( int choice ){
    heuristic_choice = choice;
}

class node{
public:
    int ** board;
    int moves;
    int blank_row;
    int blank_col;
    int f_score;
    int g_score;
    int neighbour_count;
    node ** neighbour;
    node * previous;
    node(){
        board = new int * [k];
        for( int i = 0; i < k; i++ ){
            board[i] = new int [k];
        }
        neighbour_count = 0;
        neighbour = 0;

    }

    void assignBoardValue(){
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                cin>>board[i][j];
                if( board[i][j] == -1 ){
                    blank_row = i;
                    blank_col = j;
                }
                cout<<board[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    int hamming(){
        int hammingDistance = 0;
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                if( board[i][j] != -1 && board[i][j] != i*k+j+1 ){
                    hammingDistance++;
                }
            }
        }
        //cout<<"Hamming Distance : "<<hammingDistance<<endl;
        return hammingDistance;
    }

    int manhattan(){
        int manhattanDistance = 0;
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                if( board[i][j] != -1 && board[i][j] != i*k+j+1 ){
                    int goalPosition = board[i][j] - 1;
                    int goalRow = goalPosition/k;
                    int goalColumn = goalPosition%k;
                    manhattanDistance = manhattanDistance + abs(goalRow-i) + abs(goalColumn-j);
                }
            }
        }
        //cout<<"Manhattan Distance : "<<manhattanDistance<<endl;
        return manhattanDistance;
    }

    int linearConflict(){
        int conflict = 0;
        for( int i = 0; i < k-1; i++ ){
            for( int j = 0; j < k-1; j++ ){
                if( board[i][j] != -1 ){
                    int currentValue = board[i][j];
                    int goalPosition = board[i][j] - 1;
                    int goalRow = goalPosition/k;
                    int goalColumn = goalPosition%k;
                    if( goalRow == i ){
                        for( int x = j+1; x < k; x++ ){
                            int nextGoalRow;
                            int nextValueRow;
                            if( board[i][x] != -1 ){
                                nextGoalRow = (board[i][x] - 1)/k;
                                nextValueRow = board[i][x];
                            }
                            if( nextGoalRow == goalRow && nextValueRow < currentValue ){
                                conflict++;
                                //cout<<"Conflict : "<<conflict<<endl;
                            }
                        }
                    }
                    if( goalColumn == j ){
                        for( int y = i+1; y < k; y++ ){
                            int nextGoalColumn;
                            int nextValueColumn;
                            if( board[y][j] != -1 ){
                                nextGoalColumn = (board[y][j] - 1)%k;
                                nextValueColumn = board[y][j];
                            }

                            if( nextGoalColumn == goalColumn && nextValueColumn < currentValue ){
                                conflict++;
                                //cout<<"Conflict : "<<conflict<<endl;
                            }
                        }
                    }
                }
            }
        }
        conflict = manhattan() + conflict*2;
        //cout<<"Linear conflict : "<<conflict<<endl;
        return conflict;
    }

    int heuristic(){
        if( heuristic_choice == 1 ){
            return hamming();
        }
        else if( heuristic_choice == 2 ){
            return manhattan();
        }
        else{
            return linearConflict();
        }
    }

    bool solvable(){
        int inversionCount = 0;
        int rowMajor[k*k-1];
        int index = 0;
        int blankRow;
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                if( board[i][j] != -1 ){
                    rowMajor[index] = board[i][j];
                    index++;
                }
                else{
                    blankRow = i;
                    //cout<<"Blank row : "<<blankRow<<endl;
                }
            }
        }

        int bound = k*k-1;
        for( int i = 0; i < bound-1; i++ ){
            for( int j = i+1; j < bound; j++ ){
                if( rowMajor[j] < rowMajor[i] ){
                    inversionCount++;
                }
            }
        }
        //cout<<"Inversion count : "<<inversionCount<<endl;

        //For even size board
        if( k%2 == 0 ){
            if( ( inversionCount + blankRow ) % 2 == 0 ){
                cout<<"Unsolvable"<<endl;
                return false;
            }
            else{
                cout<<"Solvable"<<endl;
                return true;
            }
        }
        //For odd size board
        else{
            if( inversionCount%2 == 0 ){
                cout<<"Solvable"<<endl;
                return true;
            }
            else{
                cout<<"Unsolvable"<<endl;
                return false;
            }
        }
    }

    bool is_goal(){
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                if( i == k-1 && j == k-1 ){
                    return true;
                }
                if( board[i][j] != i*k+j+1 ){
                    return false;
                }
            }
        }
    }

    void create_neighbour(){
        if( blank_row < k-1 )
            neighbour_count++;
        if( blank_row > 0 )
            neighbour_count++;
        if( blank_col < k-1 )
            neighbour_count++;
        if( blank_col > 0 )
            neighbour_count++;

        neighbour = new node*[neighbour_count];
        for(int i=0;i<neighbour_count;i++)
            neighbour[i]=new node();

        for( int x = 0; x < neighbour_count; x++ ){
            neighbour[x]->blank_row = blank_row;
            neighbour[x]->blank_col = blank_col;
            for( int i = 0; i < k; i++ ){
                for( int j = 0; j < k; j++ ){
                    neighbour[x]->board[i][j] = board[i][j];
                }
            }
        }
        int x = 0;
        if( blank_row < k-1 ){
            neighbour[x]->board[blank_row][blank_col] = board[blank_row+1][blank_col];
            neighbour[x]->board[blank_row+1][blank_col] = -1;
            neighbour[x]->blank_row = blank_row+1;
            x++;
        }
        if( blank_row > 0 ){
            neighbour[x]->board[blank_row][blank_col] = board[blank_row-1][blank_col];
            neighbour[x]->board[blank_row-1][blank_col] = -1;
            neighbour[x]->blank_row = blank_row-1;
            x++;
        }
        if( blank_col < k-1 ){
            neighbour[x]->board[blank_row][blank_col] = board[blank_row][blank_col+1];
            neighbour[x]->board[blank_row][blank_col+1] = -1;
            neighbour[x]->blank_col = blank_col+1;
            x++;
        }
        if( blank_col > 0 ){
            neighbour[x]->board[blank_row][blank_col] = board[blank_row][blank_col-1];
            neighbour[x]->board[blank_row][blank_col-1] = -1;
            neighbour[x]->blank_col = blank_col-1;
        }
        for( int i = 0; i < neighbour_count; i++ ){
            //neighbour[i]->print_board();
        }
    }

    void print_board(){
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                cout<<board[i][j]<<' ';
            }
            cout<<endl;
        }
        cout<<endl;
    }

    void path_print( node * n ){
        node * temp = n;
        while( temp->previous != 0 ){
            temp = temp->previous;
            temp->print_board();
        }
    }
};

class comparator{
public:
    bool operator()( node* n1, node* n2 ){
        return ( n1->f_score > n2->f_score );
    }
};

node* searchOpenList( priority_queue<node*,vector<node*>,comparator> pq, node* search_node ){
    priority_queue<node*,vector<node*>,comparator> temp_pq = pq;
    while( !temp_pq.empty() ){
        node * top = temp_pq.top();
        int unequal_board = 0;
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                if( top->board[i][j] != search_node->board[i][j] ){
                    unequal_board = 1;
                    break;
                }
            }
            if( unequal_board != 0 )
                break;
        }
        if( unequal_board == 0 )
            return top;
        temp_pq.pop();
    }
    return 0;
}

bool search_closed_list( vector<node*> closed_list, node * search_node ){
    vector<node*> temp_list = closed_list;
     for( int x = 0; x < temp_list.size(); x++ ){
        node * top = temp_list[x];
        int unequal_value = 0;
        for( int i = 0; i < k; i++ ){
            for( int j = 0; j < k; j++ ){
                if( top->board[i][j] != search_node->board[i][j] ){
                    unequal_value = 1;
                    break;
                }
            }
            if( unequal_value != 0 )
                break;
        }
        if( unequal_value == 0 )
            return true;
    }
    return false;
}

void aStarSearch( node * n, int choice ){
    if( !n->solvable() ){
        return;
    }
    int explored = 0;
    int expanded = 0;

    set_heuristic_choice( choice );
    priority_queue<node*, vector<node*>, comparator> openList;
    vector<node*> closedList;
    n->previous = 0;
    n->g_score = 0;
    n->f_score = n->g_score + n->heuristic();
    openList.push(n);

    while( !openList.empty() ){
        node * current_node = openList.top();
        if( current_node->is_goal() ){
            cout<<"\nSolution found "<<endl;
            cout<<"Moves to reach goal : "<<current_node->g_score<<endl;
            cout<<"Nodes explored : "<<explored<<endl;
            cout<<"Nodes expanded : "<<expanded<<endl;
            current_node->print_board();
            current_node->path_print( current_node );
            return;
        }

        //current_node->print_board();
        openList.pop();
        expanded++;
        closedList.push_back(current_node);

        current_node->create_neighbour();
        for( int i = 0; i < current_node->neighbour_count; i++ ){
            if( search_closed_list( closedList, current_node->neighbour[i] ) ){
                continue;
            }
            int tentative_g_score = current_node->g_score + 1;
            int previous_g_score = INT_MAX;
            node * found = searchOpenList( openList, current_node->neighbour[i] );
            if( found ){
                previous_g_score = found->g_score;
            }

            if( !found || tentative_g_score < previous_g_score ){
                current_node->neighbour[i]->previous = current_node;
                current_node->neighbour[i]->g_score = tentative_g_score;
                current_node->neighbour[i]->f_score = current_node->neighbour[i]->g_score + current_node->neighbour[i]->heuristic();
                if( !found ){
                    openList.push( current_node->neighbour[i] );
                    explored++;
                }
            }
        }
        if( explored > 100000 ){
            cout<<"Node explored : "<<explored<<endl<<endl;
            break;
        }
    }
}

int main(){
    freopen( "input.txt", "rt", stdin );
    cout<<"Enter board size k : "<<endl;
    cin>>k;
    node * newNode=new node();
    newNode->assignBoardValue();
    cout<<endl<<endl<<"----Solving by linear conflict heuristic----"<<endl<<endl;
    aStarSearch( newNode, 3 );
    cout<<endl<<endl<<"----Solving by Manhattan distance heuristic----"<<endl<<endl;
    aStarSearch( newNode, 2 );
    cout<<endl<<endl<<"----Solving by Hamming distance heuristic----"<<endl<<endl;
    aStarSearch( newNode, 1 );
    return 0;
}
