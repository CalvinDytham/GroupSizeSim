/*-------------------------------------------------------------------------------------------------
               Evolutionary model 2
-------------------------------------------------------------------------------------------------*/


#include <iostream> // input-output control
#include <conio.h> //console i/o, used for _getch()
#include <time.h>
#include <math.h>
#include <fstream>  // fstream for file output
#include <random>    //for built in C++ random numbers
#include <Windows.h> // need this to set the time, GetTickCount()

using namespace std;

//++++++++++++++++++definitions++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SEED GetTickCount() // for better pseudo random numbers
#define MaxPopulationSize 10000		//maximum possible agents
#define InitialPopulation 100		//number of agents at the start
#define NumberOfDP 10000.0 //the number of dp on my random number, efectively controlls the size of the habbitat (allong side other values) 
#define Max_group_distance 0.2 //how close two agents have to be to be considered part of the same group.
#define FitnessBenefit pow(number_of_neighbours,AlleeAdvantage)//used in function 'assesfitnes'
#define AlleeAdvantage 0.5 // between 0 and 1, the closer to one the stronger the alle effect. adjust here 
#define FitnessCost number_of_neighbours*Competition //
#define Competition 0.25 //how great the level of competition is 
#define Time_steps 100000 //howmany times agents are allowed to move
#define dF_over_dt FitnessBenefit-FitnessCost-BMetabolicRate //the change in an agents fitness each timestep. 

//++++++++++Random Numbers+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
default_random_engine generator(SEED); //
uniform_int_distribution<int> pick_random(0, NumberOfDP);	// will generate integer 0 to 10000. pick_random(generator)/NumberOfDP is what you call when you want a random number 0 to 1
uniform_int_distribution<int> coin_flip(0, 1);	// coin_flip(generator), for 0 or 1.


// Key variables
double Agent[MaxPopulationSize][5]; //[agent number][0=x pos, 1=y pos, 2=fitnes, 3=alive?(bool), 4=number_of_neighbours]
double Stop_threshold = 0.1; //In main AND run
double Move_Distance = 0.1;//Also in main and run
int LastAgent = 0; //Keep track of last ocupied cell. If agent dies then last agent is coppied into their place and LastAgent--.

// Functions 

void initialise(void); //the setup function
void count_neighbours(void); //counts how many neighbours each individual has, stored in the array
void move(void); //moves each agent that has fitness lower than the threshold 
void update_fitness(void); //updates the fitnes (resources) of each agent
void death(void); //kills 1 in 10 agents
void birth(void); //


int main()
{
	initialise();

}

void initialise(){
	cout << "initialise\n";

	for (int x = 0; x<InitialPopulation; x++){
		Agent[x][0] = pick_random(generator) / NumberOfDP;
		Agent[x][1] = pick_random(generator) / NumberOfDP;
		Agent[x][2] = 1.5; //an arbitrary value. p(death) increases below a certain value, say 1 for now, and p(bith) increases above 2?
		Agent[x][3] = 1; //this agent is alive.
		LastAgent = x;
	}
}

void count_neighbours(){
	cout << "count_neighbours\n";

	cout << "Last agent= " << LastAgent << "\n";

	int number_of_neighbours = 0;

	for (int x = 0; x < LastAgent; x++){ //note 'x=lastAgent'
		number_of_neighbours = 0;

		for (int y = 0; y < LastAgent; y++){


			if (abs(Agent[x][0] - Agent[y][0]) < Max_group_distance  //if it is within the square
				&& abs(Agent[x][1] - Agent[y][1]) < Max_group_distance){//without crossing the edge

				double x1 = Agent[x][0];
				double x2 = Agent[y][0];
				double y1 = Agent[x][1];
				double y2 = Agent[y][1];

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//x2+1
			else if (abs(Agent[x][0] - Agent[y][0] + 1) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1]) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0] + 1;
				double y1 = Agent[x][1];
				double y2 = Agent[y][1];

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//x2-1
			else if (abs(Agent[x][0] - Agent[y][0] - 1) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1]) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0] - 1;
				double y1 = Agent[x][1];
				double y2 = Agent[y][1];

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//y2+1
			else if (abs(Agent[x][0] - Agent[y][0]) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1] + 1) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0];
				double y1 = Agent[x][1];
				double y2 = Agent[y][1] + 1;

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//y2-1
			else if (abs(Agent[x][0] - Agent[y][0]) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1] - 1) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0];
				double y1 = Agent[x][1];
				double y2 = Agent[y][1] - 1;

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//x2+1, y2+1
			else if (abs(Agent[x][0] - Agent[y][0] + 1) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1] + 1) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0] + 1;
				double y1 = Agent[x][1];
				double y2 = Agent[y][1] + 1;

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//x2+1, y2-1
			else if (abs(Agent[x][0] - Agent[y][0] + 1) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1] - 1) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0] + 1;
				double y1 = Agent[x][1];
				double y2 = Agent[y][1] - 1;

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//x2-1, y2+1
			else if (abs(Agent[x][0] - Agent[y][0] - 1) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1] + 1) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0] - 1;
				double y1 = Agent[x][1];
				double y2 = Agent[y][1] + 1;

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
			//x2-1, y2-1
			else if (abs(Agent[x][0] - Agent[y][0] - 1) < Max_group_distance
				&& abs(Agent[x][1] - Agent[y][1] - 1) < Max_group_distance){

				double x1 = Agent[x][0];
				double x2 = Agent[y][0] - 1;
				double y1 = Agent[x][1];
				double y2 = Agent[y][1] - 1;

				if ((pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5)) < 0.01)number_of_neighbours++;
			}
		}

		number_of_neighbours--; //because an agent can't be it's own neibour.
		Agent[x][4] = number_of_neighbours;
	}
}

void move(){
	cout << "move\n";

	for (int x = 0; x < LastAgent; x++){
		int number_of_neighbours = Agent[x][4];
		if (FitnessBenefit - FitnessCost < Stop_threshold){
			int coin = 0;

			double radius2 = pow(Move_Distance, 2);
			double x_translation = (pick_random(generator) / NumberOfDP); //random number between 0 and 1
			double y_translation = 1 - x_translation;
			x_translation = x_translation*radius2;
			y_translation = y_translation*radius2;
			x_translation = pow(x_translation, 0.5);
			y_translation = pow(y_translation, 0.5);

			coin = coin_flip(generator);
			if (coin == 1)x_translation = x_translation*-1;	//random direction for the x translation 
			coin = coin_flip(generator);
			if (coin == 1)y_translation = y_translation*-1;	//...and forthe y translation

			Agent[x][0] = Agent[x][0] + x_translation;
			Agent[x][1] = Agent[x][1] + y_translation;

			if (Agent[x][0]>1) Agent[x][0] = Agent[x][0] - 1;
			if (Agent[x][1]>1) Agent[x][1] = Agent[x][1] - 1;
			if (Agent[x][0] < 0) Agent[x][0]++;
			if (Agent[x][1] < 0) Agent[x][1]++;


		}
	}
}

void update_fitness(){
	cout << "update_fitness\n";

	for (int x = 0; x < LastAgent; x++){
		int number_of_neighbours = Agent[x][4];
		Agent[x][2] += FitnessBenefit - FitnessCost;
	}
}

void death(){
	cout << "death\n";

	for (int x = 0; x < LastAgent; x++){		
		int y = pick_random(generator) / NumberOfDP;
		if (y < 0.1) Agent[x][3] = 0;                //every agent hat 1/10 chance of dying 
		}


	for (int x = 0; x < LastAgent; x++){	//!!!updating LastAgent INSIDE this loop is bad form and might cause problems, but possibly quicker than doing it propperly.
		if (Agent[LastAgent][3] = 1){		//As long as LastAgent is allive...
			if (Agent[x][3] = 0){			//...if there is an empty spece...
				for (int y = 0; y < 5; y++) Agent[x][y] = Agent[LastAgent][y]; //...coppy LastAgent to the empty space.(magic number 5 == elements in Agent[][y])
				LastAgent--; //
			}
		}
		else{ LastAgent--; x--; }//if last agent is dead update LastAgent and check Agent[x][] again!		(...this is the way you're not supposed to make loops, effectively this is a 'while' statement?)
	}

}

void birth(){
	cout << "birth\n";

	int OldLastAgent = LastAgent;
	for (int x = 0; x < OldLastAgent; x++){
		if ((Agent[x][2] > 3) && (LastAgent<MaxPopulationSize - 1)){

			Agent[x][2] -= 1.5;
			LastAgent++;
			Agent[LastAgent][0] = Agent[x][0]; //start posiotion is the mothers coordinates 
			Agent[LastAgent][1] = Agent[x][1];
			Agent[LastAgent][2] = 1.5; //has starting fitnes
			Agent[LastAgent][3] = 1; //is alive

		}
	}

}
