
// My2048Dlg.h : ͷ�ļ�
//������ѧ��C++���Գ�����ơ��γ���ҵ
//1400015962 ��˳�

#pragma once

#include <deque>
#include <fstream>
#include "afxwin.h"

// CMy2048Dlg �Ի���
class CMy2048Dlg : public CDialog
{
// ����
public:
	CMy2048Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY2048_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	int Matrix[4][4];//��Ϸ����(���������TileTraits�е���ţ���ָ��)
	int Last[4][4];//�������ǰ��������ʵ�ֳ���
				   //Matrix[col][row]:
				   //00 10 20 30
				   //01 11 21 31
				   //02 12 22 32
				   //03 13 23 33

	int Score;//��Ϸ����
	bool isUndoValid;//�ɷ���г�������
	bool isFail;//�Ƿ���Ϸʧ�ܣ��������ƶ���

	//���ֿ����ɫ����
	struct TileTraits {
		unsigned int num;//��ʾ������
		COLORREF backColor;//����ɫ
		COLORREF frontColor;//ǰ��ɫ
		int fontHeight;//����߶�
		int fontWidth;//������
	};
	const TileTraits tile[17] = {
		1,			RGB(205, 193, 180),		RGB(205, 193, 180),		1,		1,
		2,			RGB(238, 228, 218),		RGB(119, 110, 100),		66,		28,
		4,			RGB(237, 224, 200),		RGB(119, 110, 100),		66,		28,
		8,			RGB(242, 177, 121),		RGB(249, 246, 242),		66,		28,
		16,			RGB(245, 148, 100),		RGB(249, 246, 242),		63,		25,
		32,			RGB(246, 124, 95),		RGB(249, 246, 242),		63,		25,
		64,			RGB(246, 94, 59),		RGB(249, 246, 242),		63,		25,
		128,		RGB(237, 207, 114),		RGB(249, 246, 242),		60,		20,
		256,		RGB(237, 204, 97),		RGB(249, 246, 242),		60,		20,
		512,		RGB(237, 200, 80),		RGB(249, 246, 242),		60,		20,
		1024,		RGB(237, 197, 63),		RGB(249, 246, 242),		52,		16,
		2048,		RGB(43, 132, 98),		RGB(249, 246, 242),		52,		16,
		4096,		RGB(250, 56, 108),		RGB(249, 246, 242),		52,		16,
		8192,		RGB(129, 148, 200),		RGB(249, 246, 242),		52,		16,
		16384,		RGB(255, 0, 0),			RGB(249, 246, 242),		52,		16,
		32768,		RGB(0, 255, 0),			RGB(249, 246, 242),		52,		16,
		65536,		RGB(128, 128, 0),		RGB(249, 246, 242),		52,		16,
	};
	
	enum ARROW { LEFT, RIGHT, UP, DOWN };//�ĸ����ƶ������ö������
	//��������
	void putNewTile();//������з���һ���¿飨2��4��
	void MoveAlong(ARROW);//��һ�������ƶ�������Ϊ�ƶ�����
};
