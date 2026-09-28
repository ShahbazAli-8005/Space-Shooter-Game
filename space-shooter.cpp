#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <fstream>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define MIN_WIDTH 70
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int birdPos = MIN_WIDTH/2;
int bullets[20][4];
int bulletsLife[20];
int bulletIdx = 0;

int enemyX[3];
int enemyY[3];
int enemyFlag[3];

struct player{
    int score;
    string name;
}myPlayer;

char bird[3][5] = {
    ' ', ' ', '#', ' ', ' ',
    '|', '#', '#', '#', '|',
    '#', '#', '#', '#', '#'
};

void gotoRowCol(int rpos, int cpos){
	CursorPosition.Y = rpos;
	CursorPosition.X = cpos;
	SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

void sleep(int m)
{
        for(int j=0;j<m*21000;j++)
        {

        }
}

void leftToRight(int loop, int row, int col){
    for(int i= 0; i<loop; i++){
        gotoRowCol(row,col);
        cout<<"&";
        col++;
    }
}

void topToBottom(int loop, int row, int col){
    for(int i= 0; i<loop; i++){
        gotoRowCol(row,col);cout<<"%";
        row++;
    }
}



void drawBorder(){
    for(int i=0; i<SCREEN_HEIGHT; i++){
        gotoRowCol(i,MIN_WIDTH);cout<<"&";
    }
    for(int i=0; i<SCREEN_HEIGHT; i++){
        gotoRowCol(i,SCREEN_WIDTH);cout<<"&";
    }
    for(int i=0; i<=SCREEN_WIDTH; i++){
        gotoRowCol(SCREEN_HEIGHT,i);cout<<"&";
    }
}

void printBird(){
    for(int i=0; i<3; i++){
        for(int j=0; j<5; j++){
            gotoRowCol(i+22,j+birdPos);cout<<bird[i][j];
        }
        cout<<endl;
    }
}

void eraseBird(){
    for(int i=0; i<3; i++){
        for(int j=0; j<5; j++){
            gotoRowCol(i+22,j+birdPos);cout<<" ";
        }
        cout<<endl;
    }
}

void generateBullets(){
    bullets[bulletIdx][0] = 22;
    bullets[bulletIdx][1] = birdPos;
    bullets[bulletIdx][2] = 22;
    bullets[bulletIdx][3] = birdPos+4;
    bulletIdx++;
    if(bulletIdx==20)
        bulletIdx=0;
}

void drawBullets(){
    for(int i=0; i<20; i++){
        if(bullets[i][0]>1){
            gotoRowCol(bullets[i][0], bullets[i][1]);cout<<".";
            gotoRowCol(bullets[i][2], bullets[i][3]);cout<<".";
        }
    }
}

void eraseBullets(){
    for(int i=0; i<20; i++){
        if(bullets[i][0]>=1){
            gotoRowCol(bullets[i][0], bullets[i][1]);cout<<" ";
            gotoRowCol(bullets[i][2], bullets[i][3]);cout<<" ";
        }
    }
}

void moveBullets(){
    for(int i=0; i<20; i++){
        if(bullets[i][0] > 2)
            bullets[i][0]--;
        else
            bullets[i][0] = 0;
        if(bullets[i][2] > 2)
            bullets[i][2]--;
        else
            bullets[i][2] = 0;
    }
}

void eraseBullet(int idx){
    gotoRowCol(bullets[idx][0], bullets[idx][1]);cout<<" ";
    gotoRowCol(bullets[idx][2], bullets[idx][3]);cout<<" ";
}

void generateEnemy(int idx){
    enemyX[idx] = 3+rand()%(MIN_WIDTH-10);
}

void drawEnemy(int idx){
    if(enemyFlag[idx]==true){
    gotoRowCol(enemyY[idx],enemyX[idx]);cout<<"OO";
    gotoRowCol(enemyY[idx]+1,enemyX[idx]);cout<<"";
    gotoRowCol(enemyY[idx]+2,enemyX[idx]);cout<<"";
    gotoRowCol(enemyY[idx]+3,enemyX[idx]);cout<<"OO";
    }
}

void eraseEnemy(int idx){
    if(enemyFlag[idx]==true){
    gotoRowCol(enemyY[idx],enemyX[idx]);cout<<"   ";
    gotoRowCol(enemyY[idx]+1,enemyX[idx]);cout<<"   ";
    gotoRowCol(enemyY[idx]+2,enemyX[idx]);cout<<"   ";
    gotoRowCol(enemyY[idx]+3,enemyX[idx]);cout<<"   ";
    }
}

void resetEnemy(int idx){
    eraseEnemy(idx);
    enemyY[idx] = 4;
    generateEnemy(idx);
}

int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - birdPos >= 0 && enemyX[0] + 4 - birdPos < 8  ){
			return 1;
		}
	}
	return 0;
}

int bulletHit(){
    for(int i=0; i<20; i++){
        for(int j=0; j<4; j+=2){
            if(bullets[i][j]!=0){
                if(bullets[i][j]>=enemyY[0] && bullets[i][j]<enemyY[0]+4){
                    if(bullets[i][j+1] >= enemyX[0] &&  bullets[i][j+1] <= enemyX[0]+4){
                        eraseBullet(i);
                        bullets[i][j] = 0;
                        resetEnemy(0);
                        return 1;
                    }
                }
                if(bullets[i][j]>=enemyY[1] && bullets[i][j]<enemyY[1]+4){
                    if(bullets[i][j+1] >= enemyX[1] &&  bullets[i][j+1] < enemyX[1]+4){
                        eraseBullet(i);
                        bullets[i][j] = 0;
                        resetEnemy(1);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void updateScore(){
    gotoRowCol(7,MIN_WIDTH+7);
    cout<<"Score: "<<myPlayer.score<<endl;
}

void developer(){
    gotoRowCol(5,60);cout<<"......................."<<endl;
    gotoRowCol(6,60);cout<<"| Game Developer Info |"<<endl;
    gotoRowCol(7,60);cout<<"......................."<<endl;
    gotoRowCol(9,60);cout<<"Team Members: "<<endl;
    gotoRowCol(10,60);cout<<"Shahbaz Ali"<<endl;
    gotoRowCol(11,60);cout<<"Barera Ijaz"<<endl;
    gotoRowCol(12,60);cout<<"Sundeep Kumar"<<endl;
}
void layout(){
    developer();
    leftToRight(100,20,0);
    topToBottom(20,0,57);
    topToBottom(20,0,99);
}
void head(){
    gotoRowCol(2, MIN_WIDTH + 5);cout<<"Space Shooter";
	gotoRowCol(4, MIN_WIDTH + 6);cout<<"----------";
	gotoRowCol(6, MIN_WIDTH + 6);cout<<"----------";
	gotoRowCol(12, MIN_WIDTH + 7);cout<<"Control ";
	gotoRowCol(13, MIN_WIDTH + 7);cout<<"-------- ";
	gotoRowCol(14, MIN_WIDTH + 2);cout<<" A Key - Left";
	gotoRowCol(15, MIN_WIDTH + 2);cout<<" D Key - Right";
	gotoRowCol(16, MIN_WIDTH + 2);cout<<" Spacebar = Shoot";
	gotoRowCol(19, MIN_WIDTH + 2);cout<<" Arrow Keys can";
	gotoRowCol(20, MIN_WIDTH + 2);cout<<" be used to move";
	gotoRowCol(21, MIN_WIDTH + 2);cout<<" Bird...";

	gotoRowCol(5,10);cout<<"Press any key to start";
	getch();
	gotoRowCol(5,10);cout<<"                      ";
}
void instructionFun(){
    system("cls");
    layout();
    gotoRowCol(3,5);cout<<"Instructions"<<endl;
    gotoRowCol(4,5);cout<<"............"<<endl;
    gotoRowCol(5,5);cout<<"1. press spacebar to make bird fly: "<<endl;
    gotoRowCol(6,5);cout<<"2. Bird can be control by a,A,d,D and left-right"<<endl;
    gotoRowCol(7,5);cout<<"   arrow keys:"<<endl;
    gotoRowCol(8,5);cout<<"3. press any key to go back to menu: "<<endl;
    getch();
}

void createFile(){
    system("cls");
    ofstream myFile;
    myFile.open("login.txt", ios::app);
    myFile<<endl<<myPlayer.name<<" has scored: "<<myPlayer.score;
    myFile.close();

    layout();
    string st;
    ifstream readMyFile("login.txt");
    gotoRowCol(4,1);
    while(getline(readMyFile,st)){
        cout<<st<<endl;
    }
    getch();
    readMyFile.close();
}

void gameOver(){
    system("cls");
	cout<<endl;
	layout();
	gotoRowCol(5,5);cout<<"\t\t--------------------------";
	gotoRowCol(6,5);cout<<"\t\t|        Game Over       |";
	gotoRowCol(7,5);cout<<"\t\t--------------------------";

	gotoRowCol(10,16);
	cout<<myPlayer.name<<" has scored "<<myPlayer.score;

    gotoRowCol(11,5);cout<<"\t\tPress any key to go back to menu.";
    getch();
}

int ask_for_level(){
    system("cls");
    layout();

    gotoRowCol(4,10);
    cout<<"Enter Your Name: ";
    getline(cin,myPlayer.name);

    int speed;
    char level;
    gotoRowCol(5,10);cout<<"Enter level 1,2,3,4,5: you want to Play: ";
    level = getche();

    system("cls");
    layout();

    //to select level
    switch(level){
        case '1': speed = 5000; break;
        case '2': speed = 4000; break;
        case '3': speed = 3000; break;
        case '4': speed = 2000; break;
        case '5': speed = 1000; break;
        default: cout<<"Only five level available, Enter again:: "; ask_for_level(); break;
    }
    return speed;
}

void playGame(){
    birdPos = -1 + MIN_WIDTH/2;
    myPlayer.score = 0;
    enemyFlag[0]=1;
    enemyFlag[1]=1;
    enemyY[0]=enemyY[1]=1;

    for(int i=0; i<20; i++){
        bullets[i][0] = bullets[i][1] = 0;
    }

    int speed = ask_for_level();
    system("cls");

    system("cls");
    drawBorder();
    generateEnemy(0);
    generateEnemy(1);
    updateScore();
    head();

    while(1){
        if(kbhit()){
            char ch = getch();
            if(ch=='a' || ch == 'A' || ch == 75){
                if(birdPos > 2){
                    birdPos -= 2;
                }
            }
            if(ch=='d' || ch == 'D' || ch == 77){
                if(birdPos < MIN_WIDTH-7){
                    birdPos += 2;
                }
            }
            if(ch==32)
                generateBullets();
            if(ch==27){
                break;
            }
        }

        printBird();
        drawEnemy(0);
        drawEnemy(1);
        drawBullets();

		if( collision() == 1  ){
			gameOver();
			return;
		}

        if(bulletHit()==1){
            myPlayer.score++;
            updateScore();
        }
        sleep(speed);
        eraseBird();
        eraseEnemy(0);
        eraseEnemy(1);
        eraseBullets();
        moveBullets();

        if(enemyFlag[0] ==1 )
            enemyY[0] += 1;
        if(enemyFlag[1] == 1)
            enemyY[1] += 1;
        if(enemyY[0] > SCREEN_HEIGHT-5)
            resetEnemy(0);
        if(enemyY[1] > SCREEN_HEIGHT-5)
            resetEnemy(1);

    }
}

int main(){
    ofstream myFile("Login.txt");
    setcursor(0,0);
    do{
        char option;
        system("cls");
        layout();

		gotoRowCol(5,10); cout<<" -------------------------- ";
		gotoRowCol(6,10); cout<<" |     Space Shooter      | ";
		gotoRowCol(7,10); cout<<" --------------------------";
		gotoRowCol(9,10); cout<<"1. Start Game";
		gotoRowCol(10,10); cout<<"2. Instructions";
		gotoRowCol(11,10); cout<<"3. To see your all performance";
		gotoRowCol(12,10); cout<<"4. Quit";
		gotoRowCol(13,10); cout<<"Select option: ";
		option = getche();

		if(option == '1') playGame();
		else if(option == '2') instructionFun();
		else if(option == '3') createFile();
		else if(option == '4') exit(0);
    }while(1);

    gotoRowCol(30,1);
    return 0;
}
