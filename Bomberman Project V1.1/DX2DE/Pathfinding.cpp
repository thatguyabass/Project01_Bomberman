////--------------------------------------------------
////Pathfinding.cpp
////--------------------------------------------------
////
//
//#include "Pathfinding.h"
//
//Pathfinding::Pathfinding()
//{
//	m_baseG = 1;
//
//	m_pClosed.reserve(RESERVE);
//	m_pOpen.reserve(RESERVE);
//	m_pPath.reserve(RESERVE);
//}
//
//Pathfinding::~Pathfinding()
//{
//	Shutdown();
//}
//
//int Pathfinding::LowestFScore(Node* target)
//{
//	if(m_pOpen.empty())
//		return -1;
//
//	//Calculate the H and F Scores
//	for(int c = 0; c < m_pOpen.size(); c++)
//		m_pOpen[c]->SetScores(target);
//
//	int f = m_pOpen[0]->GetF();
//	int i = 0;
//
//	for(int c = 1; c < m_pOpen.size(); c++)
//	{
//		if(f >= m_pOpen[c]->GetF())
//		{
//			f = m_pOpen[c]->GetF();
//			i = c;
//		}
//	}
//
//	return i;
//}
//
//void Pathfinding::Shutdown()
//{
//	for(int c = 0; c < m_pClosed.size(); c++)
//		delete m_pClosed[c];
//
//	for(int c = 0; c < m_pOpen.size(); c++)
//		delete m_pOpen[c];
//
//	for(int c = 0; c < m_pPath.size(); c++)
//		delete m_pPath[c];
//}