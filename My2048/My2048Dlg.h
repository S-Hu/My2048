
// My2048Dlg.h : 头文件
//北京大学《C++语言程序设计》课程作业
//1400015962 胡顺昕

#pragma once

#include <deque>
#include <fstream>
#include "afxwin.h"

// CMy2048Dlg 对话框
class CMy2048Dlg : public CDialog
{
// 构造
public:
	CMy2048Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY2048_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	int Matrix[4][4];//游戏矩阵(保存的是在TileTraits中的序号，即指数)
	int Last[4][4];//保存操作前矩阵，用于实现撤销
				   //Matrix[col][row]:
				   //00 10 20 30
				   //01 11 21 31
				   //02 12 22 32
				   //03 13 23 33

	int Score;//游戏分数
	bool isUndoValid;//可否进行撤销操作
	bool isFail;//是否游戏失败（不可再移动）

	//数字块的颜色参数
	struct TileTraits {
		unsigned int num;//显示的数字
		COLORREF backColor;//背景色
		COLORREF frontColor;//前景色
		int fontHeight;//字体高度
		int fontWidth;//字体宽度
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
	
	enum ARROW { LEFT, RIGHT, UP, DOWN };//四个块移动方向的枚举类型
	//两个操作
	void putNewTile();//向矩阵中放置一个新块（2或4）
	void MoveAlong(ARROW);//沿一个方向移动，参数为移动方向
};
