//************************************  PROGRAM IDENTIFICATION  ***************************************
//*                                                                                                   *
//*   PROGRAM FILE NAME:  Project1.cpp          ASSIGNMENT #:  1            Grade: _________          *
//*                                                                                                   *
//*   PROGRAM AUTHOR: _______________________________________________________                         *
//*                                    Yuki Tsukamoto                                                 *
//*                                                                                                   *
//*   COURSE #:  CSC 36000 11                              DUE DATE:  February 05, 2020                *
//*                                                                                                   *
//*****************************************************************************************************
//***********************************  PROGRAM DESCRIPTION  *******************************************
//*                                                                                                   *
//*   PROCESS: This program assign seats to passengers for Southern Comfort Airlines. It reads in     *
//*   passenger data then assign seats for them on various flights according to the rules.            *
//*   #Rule 1: Specific seat requests are honored first.                                              * 
//*   #Rule 2: If a requested seat is already assigned to another passenger, the person should be     *
//*    place in the same row, if possible, starting from the left side (lowest seat number).          *
//*   #Rule 3: If the requested row is full, the passenger should be seated as far forward as         *
//*    possible in the requested column but within the requested section.                             *
//*   #Rule 4: If the request using Rule 2 or Rule 3 cannot be honored, the passenger should be       * 
//*    assigned the first available seat starting from the front and left of the requested section    *
//*    going across each row until a vacant seat is found.                                            * 
//*   #Rule 5: First Class and Coach request must be honored.  If there are no seats in the requested *
//*    section, the passenger's Boarding Number must go on a waiting list for the next flight.        * 
//*                                                                                                   * 
//*****************************************************************************************************


#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

struct PassengerType
{
	int boardNum = 0;
	int flightNum = 0;
	string sec;
	int row = 0;
	string col;
	int flight = 0;
	int column = 0;
};

struct WaitType
{
	int One = 0;
	int Two = 0;
	int Three = 0;
	int Four = 0;
	int Five = 0;
	int Six = 0;
	int Seven = 0;
	int Eight = 0;

};
//*********************************Prototypes*******************************************
void buildSeating(int[8][10][3]);
void convertFlight(PassengerType&);
void convertRow(PassengerType&);
void convertColumn(PassengerType&);
void searchOne(int[8][10][3], bool&, PassengerType);
void searchTwo(int[8][10][3], bool&, PassengerType);
void searchThree(int[8][10][3], bool&, PassengerType);
void searchFour(int[8][10][3], bool&, PassengerType);
void printChart(ofstream&, int[8][10][3], int);
void Header(ofstream&);
void Footer(ofstream&);


//**************************************************************************************


void buildSeating(int Chart[8][10][3])
{
	// Receives- Chart array
	// Task    - put empty passenger numbers into array
	// Returns - nothing
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 3; k++)
			{
				Chart[i][j][k] = -999;
			}
}

void convertRow(PassengerType& passenger)
{
	//Receives - Passenger
	//Task - Convert the row number in order to make it fit in the array
	//Returns - nothing
	passenger.row -= 1;
}

void convertFlight(PassengerType& passenger)
{
	// Receives- Passenger
	// Task    - Convert the flight number into an index for use in the Chart array
	// Returns - nothing
	switch (passenger.flightNum) {
		case 1010:
			passenger.flight = 0;
			break;
		case 1015:
			passenger.flight = 1;
			break;
		case 1020:
			passenger.flight = 2;
			break;
		case 1025:
			passenger.flight = 3;
			break;
		case 1030:
			passenger.flight = 4;
			break;
		case 1035:
			passenger.flight = 5;
			break;
		case 1040:
			passenger.flight = 6;
			break;
		case 1045:
			passenger.flight = 7;
			break;
	}
}
void convertColumn(PassengerType& passenger)
{
	// Receives- passenger
	// Task    - Convert the column letter into an index for use in the Chart array
	// Returns - passenger
	if (passenger.col == "L")
		passenger.column = 0;
	if (passenger.col == "M")
		passenger.column = 1;
	if (passenger.col == "R")
		passenger.column = 2;
}

void searchOne(int Chart[8][10][3], bool& searchAgain, PassengerType passenger)
{
	// Receives- Chart array, searchAgain, passenger
	// Task    - Search for the specific seat and place them in the spot
	// Returns - nothing
	if (Chart[passenger.flight][passenger.row][passenger.column] == -999)
		Chart[passenger.flight][passenger.row][passenger.column] = passenger.boardNum;
	else
	{
		searchAgain = true;
	}
}


void searchTwo(int Chart[8][10][3], bool& searchAgain, PassengerType passenger)
{
	// Receives- Chart array, searchAgain, passenger
	// Task    - Search for an empty seat in the row from left to right
	//           place the passenger in the first available seat
	// Returns - nothing
	for (int i = 0; i < 3; i++)
	{
		if (Chart[passenger.flight][passenger.row][i] == -999)
		{
			Chart[passenger.flight][passenger.row][i] = passenger.boardNum;
			passenger.column = i;
			searchAgain = false;
			return;
		}
		else
		{
			searchAgain = true;
		}
	}
}

void searchThree(int Chart[8][10][3], bool& searchAgain, PassengerType passenger)
{
	// Receives- Chart array, searchAgain, passenger
	// Task    - search the column for the farthest forward seat
	//           places the passenger in the seat
	// returns - nothing
	if (passenger.sec == "F")
	{
		for (int i = 0; i < 3; i++)
			if (Chart[passenger.flight][i][passenger.column] == -999)
			{
				Chart[passenger.flight][i][passenger.column] = passenger.boardNum;
				passenger.row = i;
				searchAgain = false;
				return;
			}
			else
			{
				searchAgain = true;
			}
	}
	if (passenger.sec == "C")
	{
		for (int i = 3; i < 10; i++)
			if (Chart[passenger.flight][i][passenger.column] == -999)
			{
				Chart[passenger.flight][i][passenger.column] = passenger.boardNum;
				passenger.row = i;
				searchAgain = false;
				return;
			}
			else
			{
				searchAgain = true;
			}
	}
}


void searchFour(int Chart[8][10][3], bool& searchAgain, PassengerType passenger)
{
	// Receives- Chart array, searchAgain, passenger
	// Task    - search all seats in requested section until and empty seat is found
	//           place passenger in empty seat
	// Returns - nothing
	if (passenger.sec == "F")
	{
		//row
		for (int i = 0; i < 3; i++)
			//col
			for (int j = 0; j < 3; j++)
				if (Chart[passenger.flight][i][j] == -999)
				{
					Chart[passenger.flight][i][j] = passenger.boardNum;
					passenger.row = i;
					passenger.col = j;
					searchAgain = false;
					return;
				}
				else
				{
					searchAgain = true;
				}
	}
	if (passenger.sec == "C")
	{
		//row
		for (int i = 3; i < 10; i++)
			//col
			for (int j = 0; j < 3; j++)
				if (Chart[passenger.flight][i][j] == -999)
				{
					Chart[passenger.flight][i][j] = passenger.boardNum;
					passenger.row = i;
					passenger.col = j;
					searchAgain = false;
					return;
				}
				else
				{
					searchAgain = true;
				}
	}



}


void printChart(ofstream& output, int Chart[8][10][3], int which)
{
	// Receives- Output file, Chart array, which
	// Task    - Print out the specified seating chart
	// Returns - nothing
	for (int i = 0; i < 10; i++)
	{
		output << "\t";
		for (int j = 0; j < 3; j++)
		{
			output << Chart[which][i][j] << "    ";
		}
		output << endl;
	}



}






//*************************************  FUNCTION HEADER  **********************************************
void Header(ofstream& Outfile) {
	// Receives – the output file               
	// Task - Prints the output preamble               
	// Returns - Nothing   
	Outfile << setw(30) << "Yuki Tsukamoto";
	Outfile << setw(17) << "CSC 36000";
	Outfile << setw(15) << "Section 11" << endl;
	Outfile << setw(30) << "Spring 2020";
	Outfile << setw(20) << "Assignment #1" << endl;
	Outfile << setw(35) << "-----------------------------------";
	Outfile << setw(35) << "-----------------------------------" << endl << endl;
	return;
}

//*************************************  FUNCTION FOOTER  **********************************************
void Footer(ofstream& Outfile) {
	// Receives – the output file               
	// Task - Prints the output salutation               
	// Returns - Nothing   Outfile << endl;   
	Outfile << setw(35) << " --------------------------------- " << endl;
	Outfile << setw(35) << "|      END OF PROGRAM OUTPUT      |" << endl;
	Outfile << setw(35) << " --------------------------------- " << endl;
	return;
}



int main()
{
	// Create an inoput stream for an incoming passenger file containing entries
	ifstream input("data1.txt", ios::in);
	// Create and output stream for the finished seating chart
	ofstream output("Output.txt", ios::out);
	// Create the array for the seating chart
	int Chart[8][10][3] = { 0 };
	// Create the array for the waiting list
	int Wait[8][30] = { 0 };
	// Build the initial seating chart
	buildSeating(Chart);
	// Create a program stopper
	bool go = true;
	// Create a passenger used for all incoming passengers
	PassengerType passenger;
	// Create a variable for searching
	bool searchAgain = false;
	// Create a wait for Waitlist count of all flight numbers
	WaitType wait;


	Header(output);
	while (go)
	{
		input >> passenger.boardNum;
		if (passenger.boardNum == -999)
		{
			go = false;
		}
		else
		{
			input >> passenger.flightNum;
			input >> passenger.sec;
			input >> passenger.row;
			input >> passenger.col;
			convertFlight(passenger);
			convertColumn(passenger);
			convertRow(passenger);
			// Rule 1 part 1 search
			searchOne(Chart, searchAgain, passenger);
			if (searchAgain)
			{
				// Rule 1 part 2 search
				searchTwo(Chart, searchAgain, passenger);
			}
			if (searchAgain)
			{
				// Rule 2
				searchThree(Chart, searchAgain, passenger);
			}
			if (searchAgain)
			{
				searchFour(Chart, searchAgain, passenger);
			}
			if (searchAgain)
			{
				switch (passenger.flight) {
				case 0:
					Wait[passenger.flight][wait.One++] = passenger.boardNum;
					break;
				case 1:
					Wait[passenger.flight][wait.Two++] = passenger.boardNum;
					break;
				case 2:
					Wait[passenger.flight][wait.Three++] = passenger.boardNum;
					break;
				case 3:
					Wait[passenger.flight][wait.Four++] = passenger.boardNum;
					break;
				case 4:
					Wait[passenger.flight][wait.Five++] = passenger.boardNum;
					break;
				case 5:
					Wait[passenger.flight][wait.Six++] = passenger.boardNum;
					break;
				case 6:
					Wait[passenger.flight][wait.Seven++] = passenger.boardNum;
					break;
				case 7:
					Wait[passenger.flight][wait.Eight++] = passenger.boardNum;
					break;
			}

			}

		}

	}



	for (int i = 0; i < 8; i++)
	{
		if (i == 0)
		{
			output <<"\t\t\t" << "Southern Comfort Airlines" << endl << endl;
			output << "Flight 1010" << "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			output << "\t\t\t\t\t\t" << "TO: Memphis, Tennessee" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.One < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;
			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.One; i++)
				{
					output << Wait[0][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl << endl;
			}
		}
		if (i == 1)
		{
			output << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			output << "\t\t\t" << "Southern Comfort Airlines" << endl << endl;
			output << "Flight 1015" <<  "\t\t\t\t\t" << "FROM: Memphis, Tennessee" << endl;
			output << "\t\t\t\t\t\t"  << "TO: Jackson. Mississippi" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.Two < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;
			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.Two; i++)
				{
					output << Wait[1][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl << endl;
			}

		}
		if (i == 2)
		{
			output << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			output << "\t\t\t" << "Southern Comfort Airlines" << endl << endl;
			output << "Flight 1020" << "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			output << "\t\t\t\t\t\t"  << "TO: Little Rock, Arkansas" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.Three < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;
			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.Three; i++)
				{
					output << Wait[2][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl << endl;
			}

		}
		if (i == 3)
		{
			output << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			output << "\t\t\t" << "Southern Comfort Airlines" << endl << endl;
			output << "Flight 1025" <<  "\t\t\t\t\t" << "FROM: Little Rock, Arkansas" << endl;
			output << "\t\t\t\t\t\t" << "TO: Jackson. Mississippi" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.Four < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;

			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.Four; i++)
				{
					output << Wait[3][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl << endl;
			}

		}
		if (i == 4)
		{
			output << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			output << "\t\t\t" << "Southern Comfort Airlines" << endl << endl;
			output << "Flight 1030" <<  "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			output << "\t\t\t\t\t\t"  << "TO: Shreveport, Louisiana" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.Five < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;
			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.Five; i++)
				{
					output << Wait[4][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl << endl;
			}

		}
		if (i == 5)
		{
			output << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			output << "\t\t\t" << "Southern Comfort Airlines" << endl << endl;
			output << "Flight 1035" << "\t\t\t\t\t" << "FROM: Shreveport, Louisiana" << endl;
			output << "\t\t\t\t\t\t" << "TO: Jackson, Mississippi" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.Six < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;
			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.Six; i++)
				{
					output << Wait[5][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl;
			}

		}
		if (i == 6)
		{
			output << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			output << "\t\t\t" << "Southern Comfort Airlines" << endl << endl;
			output << "Flight 1040" << "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			output << "\t\t\t\t\t\t"  << "TO: Orlando, Florida" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.Seven < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;
			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.Seven; i++)
				{
					output << Wait[6][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl << endl;
			}

		}
		if (i == 7)
		{
			output << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
			output << "\t\t\t" <<"Southern Comfort Airlines" << endl << endl;
			output << "Flight 1045" <<  "\t\t\t\t\t" << "FROM: Orlando, Florida" << endl;
			output << "\t\t\t\t\t\t"  << "TO: Jackson, Mississippi" << endl << endl;

			printChart(output, Chart, i);
			output << endl;
			output << "WAITING LIST" << endl << endl;
			if (wait.Eight < 1)
			{
				output << "\tThere is no waiting list for this flight." << endl << endl;
			}
			else
			{
				int next = 0;
				for (int i = 0; i < wait.Eight; i++)
				{
					output << Wait[7][i] << "   ";
					next++;
					if (next == 10)
					{
						output << endl;
						next = 0;
					}
				}
				output << endl << endl;
			}

		}
	}


	Footer(output);
	return 0;
}