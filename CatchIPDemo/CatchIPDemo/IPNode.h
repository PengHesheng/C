#include <malloc.h>
#include <stdio.h>

typedef unsigned long ULONG;
typedef unsigned char UCHAR;

typedef struct CIPNode
{
	ULONG m_dwSourceIPAddr;  //ԴIP
	ULONG m_dwDestIPAddr;  //Ŀ��IP
	UCHAR m_chProtocol;  //Э������
	ULONG m_dwCouter;  //���ݰ�������
	int node_num;

	struct CIPNode * pNext;  //��һIP
} CIPNode, * pIPNODE;

void addNode(pIPNODE pHead, ULONG dwSourceIP, ULONG dwDestIp, UCHAR chPro);
void deleteNode(pIPNODE pHead);
pIPNODE createHead(ULONG dwSourceIP, ULONG dwDestIp, UCHAR chPro)
{
	pIPNODE pHead = (pIPNODE)malloc(sizeof(CIPNode));
	if (pHead == NULL)
	{
		return NULL;
	}
	pHead->m_dwSourceIPAddr = dwSourceIP;
	pHead->m_dwDestIPAddr = dwDestIp;
	pHead->m_chProtocol = chPro;
	pHead->m_dwCouter = 1;
	pHead->pNext = NULL;
	pHead->node_num = 1;
	return pHead;
};

void addNode(pIPNODE pHead, ULONG dwSourceIP, ULONG dwDestIp, UCHAR chPro)
{
	
	pIPNODE pTemp = pHead;
	pIPNODE pPre = pTemp;
	while (pTemp)
	{
		if (pTemp->m_dwSourceIPAddr == dwSourceIP
			&& pTemp->m_dwDestIPAddr == dwDestIp
			&& pTemp->m_chProtocol == chPro)
		{
			pTemp->m_dwCouter++;
			break;
		}
		pPre = pTemp;
		pTemp = pTemp->pNext;
	}
	if (pTemp == NULL)
	{
		pIPNODE newNode = (pIPNODE)malloc(sizeof(CIPNode));
		if (newNode == NULL) {
			return;
		}
		//β�巨
		newNode->m_dwSourceIPAddr = dwSourceIP;
		newNode->m_dwDestIPAddr = dwDestIp;
		newNode->m_chProtocol = chPro;
		newNode->m_dwCouter = 1;
		pPre->pNext = newNode;
		newNode->pNext = NULL;
		pHead->node_num++;
	}
}

void deleteNode(pIPNODE pHead)
{

}



