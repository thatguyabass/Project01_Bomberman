//---------------------------------------------------------------------
//Pathfinding.h
//---------------------------------------------------------------------
//This application uses A* pathfinding. This class handles the genreating of a path for the seeker types.
//The Formula used is F = H + G.
//F = FINAL
//H = ESTIMATED
//G = ACTUAL MOVEMENT
//Using the Nodes in the Level and Editor Grids these values are modified to get the fastest path from point A to point B
//Some of the Calculates in this class will also take place within the Class 'Enemy' and 'Base Node'

#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#include "Node.h"
#include "Enemy.h"
#include "Seeker.h"

#include<algorithm>

//----------------------------------------------------------------------------------------------------------

//Set the space to reserve for each vector
#define RESERVE 800

//S = SEEKER, N = NODE
template<class S, class N>
class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();

	//Find a path for a seeker
	void FindPath(S*);
	bool FindPath(N*, N*);//Find a path between two nodes

private:
	//Methods
	void Shutdown();
	//Generate the path for the seeker object
	void GeneratePath(S* seeker, N* origin, N* target);
	int LowestFScore(N*);//Find the lowest F score out of all the open nodes

	//Variables
	int m_baseG;//Movement value
	vector<N*> m_pOpen;//Nodes that can be checked
	vector<N*> m_pClosed;//Nodes already checked
	vector<N*> m_pPath;//Path from A - B

};

template<class S, class N>
Pathfinding<S,N>::Pathfinding()
{
	m_baseG = 1;//Base Movement is always 1

	//Reserve space in the vectors to avoid a hitch
	m_pClosed.reserve(RESERVE);
	m_pOpen.reserve(RESERVE);
	m_pPath.reserve(RESERVE);
}

template<class S, class N>
Pathfinding<S,N>::~Pathfinding()
{
	Shutdown();
}

template<class S, class N>
int Pathfinding<S,N>::LowestFScore(N* target)
{
	if(m_pOpen.empty())//No Nodes to check
		return -1;

	//Calculate the H and F Scores
	for(int c = 0; c < m_pOpen.size(); c++)
		m_pOpen[c]->SetScores(target);

	int f = m_pOpen[0]->GetF();//Base Value
	int i = 0;//Base Index

	for(int c = 1; c < m_pOpen.size(); c++)//Loop through the open list to find the lowest F score
	{
		if(f >= m_pOpen[c]->GetF())
		{
			f = m_pOpen[c]->GetF();
			i = c;
		}
	}

	return i;
}

template<class S, class N>
void Pathfinding<S,N>::FindPath(S* seeker)
{
	N* O = seeker->GetOrigin();
	N* T = seeker->GetTarget();
	N* S = O;//Current Node

	int tempG;
	bool path = false;

	seeker->ResetPath();//Clear the Seekers current path

	S->SetG(m_baseG);//Set the Initial G 
	m_pOpen.push_back(S);//Add S to the Open List
	do
	{
		//Get the next Node using the lowest F Value
		int i = LowestFScore(T);
		if(i == -1)//if -1 there are no nodes in the open list
		{
			T = m_pClosed[m_pClosed.size() - 1];
			break;
		}
		S = m_pOpen[i];

		//Add the current Space to the Closed List and remove it from the open
		m_pClosed.push_back(S);
		m_pOpen.erase(m_pOpen.begin() + i);

		//Check if target is within the closed vector
		if(find(m_pClosed.begin(), m_pClosed.end(), T) != m_pClosed.end())
			break;//Path to target found

		//Add the Adjacent Nodes of S into the open list
		for(int c = 0; c < S->NodeCount(); c++)
		{
			//Check if the Node is Walkable
			if(S->GetNode(c)->GetNodeState())
			{
				//Check if the node is already in the closed list
				if(find(m_pClosed.begin(), m_pClosed.end(), S->GetNode(c)) != m_pClosed.end())
					continue;

				//Check if this not isn't already within the open list
				if(find(m_pOpen.begin(), m_pOpen.end(), S->GetNode(c)) == m_pOpen.end())
				{
					//Add new node
					m_pOpen.push_back(dynamic_cast<N*>(S->GetNode(c)));
					tempG = S->GetG() + m_baseG;//Increase the Movement Value
					S->GetNode(c)->SetG(tempG);//Set the Movement value to the selected node
				}
				else
				{
					//Get the current G and see if this node hasa great G value
					//If true, change the current nodes G value because this node
					//is in a faster path the before
					tempG = S->GetG() + m_baseG;//Increase the Movement Value
					if(tempG < S->GetNode(c)->GetG())
						S->GetNode(c)->SetG(tempG);
				}
			}
		}

	}while(!path);

	GeneratePath(seeker, O, T);

	//Null out the Pointers
	O = 0;
	S = 0;
	T = 0;

	//Clear the Vectors for next time
	m_pClosed.clear();
	m_pOpen.clear();
	m_pPath.clear();
}

template<class S, class N>
bool Pathfinding<S,N>::FindPath(N* O, N* T)
{
	bool result = false;

	N* S = O;
	int tempG;
	bool path = false;

	S->SetG(m_baseG);
	m_pOpen.push_back(S);
	do
	{
		//Get the next Node using the lowest F Value
		int i = LowestFScore(T);
		if(i == -1)//if -1 there are no nodes in the open list
		{
			T = m_pClosed[m_pClosed.size() - 1];
			result = false;
			break;
		}
		S = m_pOpen[i];

		//Add the current Space to the Closed List and remove it from the open
		m_pClosed.push_back(S);
		m_pOpen.erase(m_pOpen.begin() + i);

		//Check if target is within the closed vector
		if(find(m_pClosed.begin(), m_pClosed.end(), T) != m_pClosed.end())
		{
			//Path to target found
			result = true;
			break;
		}

		//Add the Adjacent Nodes of S into the open list
		for(int c = 0; c < S->NodeCount(); c++)
		{
			//Check if the Node is Walkable
			if(S->GetNode(c)->GetNodeState())
			{
				if(find(m_pClosed.begin(), m_pClosed.end(), S->GetNode(c)) != m_pClosed.end())
					continue;
				if(find(m_pOpen.begin(), m_pOpen.end(), S->GetNode(c)) == m_pOpen.end())
				{
					//Add new node
					m_pOpen.push_back(dynamic_cast<N*>(S->GetNode(c)));
					tempG = S->GetG() + m_baseG;//Increase the Movement Value
					S->GetNode(c)->SetG(tempG);//Set the Movement value to the selected node
				}
				else
				{
					//Get the current G and see if this node hasa great G value
					//If true, change the current nodes G value because this node
					//is in a faster path the before
					tempG = S->GetG() + m_baseG;
					if(tempG < S->GetNode(c)->GetG())
						S->GetNode(c)->SetG(tempG);
				}
			}
		}

	}while(!path);

	//Null out the Pointers
	S = 0;

	//Clear the Vectors for next time
	m_pClosed.clear();
	m_pOpen.clear();

	return result;
}

template<class S, class N>
void Pathfinding<S,N>::GeneratePath(S* seeker, N* O, N* T)
{
	//Add the target to the path
	m_pPath.push_back(T);
	do
	{
		//Set G to a extream value so that it will always get reset
		int G = 9999;
		int I = 0;
		for(int c = 0; c < T->NodeCount(); c++)//Loop through the Adjacent nodes
		{
			//check if the node is in the closed list
			if(find(m_pClosed.begin(), m_pClosed.end(), T->GetNode(c)) != m_pClosed.end())
			{
				if(T->GetNode(c)->GetG() < G)//Find the Lowest G value
				{
					G = T->GetNode(c)->GetG();
					I = c;
				}
			}
		}

		m_pPath.push_back(dynamic_cast<N*>(T->GetNode(I)));//Add the Node with the Lowest G to the path vector
		T = dynamic_cast<N*>(T->GetNode(I));//Set T to the Node with the Lowest G

		if(T == O)//Check if T is Origin
			break;

	} while(T != 0);
	
	//Add the path to the seeker object in reverse order
	for(int c = m_pPath.size() - 1; c > -1; c--)
		seeker->AddNode(m_pPath[c]);
}

template<class S, class N>
void Pathfinding<S,N>::Shutdown()
{
	for(int c = 0; c < m_pClosed.size(); c++)
		delete m_pClosed[c];

	for(int c = 0; c < m_pOpen.size(); c++)
		delete m_pOpen[c];

	for(int c = 0; c < m_pPath.size(); c++)
		delete m_pPath[c];
}

#endif//_PATHFINDING_H_