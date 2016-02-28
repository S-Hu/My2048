
// My2048Dlg.cpp : 实现文件
//北京大学《C++语言程序设计》课程作业
//1400015962 胡顺昕

#include "stdafx.h"
#include "My2048.h"
#include "My2048Dlg.h"
#include "afxdialogex.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2048Dlg 对话框

CMy2048Dlg::CMy2048Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MY2048_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMy2048Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy2048Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RESTART, &CMy2048Dlg::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_UNDO, &CMy2048Dlg::OnBnClickedUndo)
	ON_BN_CLICKED(IDC_SAVE, &CMy2048Dlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CMy2048Dlg::OnBnClickedLoad)
END_MESSAGE_MAP()


// CMy2048Dlg 消息处理程序

BOOL CMy2048Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	srand((unsigned)time(NULL));//设置随机数种子
	OnBnClickedRestart();//新局初始化
	SetTimer(1, 100, NULL);//设置计时器（定时刷新绘图）

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy2048Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		//填充背景色
		dc.FillSolidRect(rect, RGB(250, 248, 239));
		//绘制标题
		CFont titleFont;//创建字体
		titleFont.CreateFont(80, 40, 0, 0,
			FW_BLACK, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
		CFont *pOldFont = dc.SelectObject(&titleFont);//选择字体
		dc.SetTextColor(RGB(119, 110, 101));//设置颜色
		RECT titleRect = { 40,10,230,80 };//定义绘制区域
		dc.DrawText(_T("2048"), &titleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);//绘制文本
		//释放内存
		dc.SelectObject(pOldFont);
		titleFont.DeleteObject();

		CDialog::OnPaint();
	}
}

//计时器事件，定时刷新绘制窗体
void CMy2048Dlg::OnTimer(UINT_PTR nIDEvent){
	//绘图初始化（使用双缓冲绘图，避免出现图像闪烁）
	RECT rect;
	GetClientRect(&rect);
	CDC *pDC = GetDC();
	CDC memDC;//定义显示设备对象
	CBitmap memBitmap;//定义位图对象
	memDC.CreateCompatibleDC(pDC);//建立与屏幕显示兼容的内存显示设备
	memBitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);//创建与屏幕显示兼容的位图
	CBitmap *pOldBit = memDC.SelectObject(&memBitmap);//将位图选入内存DC
	memDC.SetBkMode(TRANSPARENT);//设置字符串背景为透明
	CFont *pOldFont;

	//方块的宽度为100，方块间的间隔为10

	/*---绘制棋盘---*/
	RECT boardRect = { 40, 150, 490, 600 };
	memDC.FillSolidRect(&boardRect, RGB(187, 173, 160));

	/*---绘制磁块---*/
	for (int col = 0; col < 4; ++col)
		for (int row = 0; row < 4; ++row) {
			RECT tileRect = { 50 + col * 110, 160 + row * 110, 150 + col * 110, 260 + row * 110};//当前块位置
			const TileTraits &thisTile = tile[Matrix[col][row]];//取当前块属性
			memDC.FillSolidRect(&tileRect, thisTile.backColor);//填充背景
			//绘制数字
			if (thisTile.num != 0) {//0为空格，不画
				CFont numFont;
				numFont.CreateFont(thisTile.fontHeight, thisTile.fontWidth, 0, 0,
					FW_BLACK, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
					CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
				pOldFont = memDC.SelectObject(&numFont);
				CString num2draw;
				num2draw.Format(_T("%d"), thisTile.num);//数字转CString
				memDC.SetTextColor(thisTile.frontColor);
				memDC.DrawText(num2draw, &tileRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				memDC.SelectObject(pOldFont);
				numFont.DeleteObject();
			}
		}

	/*---绘制得分---*/
	//背景
	RECT scoreRect = { boardRect.right - 80, 20, boardRect.right, 70 };
	memDC.FillSolidRect(&scoreRect, RGB(187, 173, 160));
	//文字
	CFont scoreFont;
	scoreFont.CreateFont(22, 9, 0, 0,
		FW_BLACK, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
	pOldFont = memDC.SelectObject(&scoreFont);
	memDC.SetTextColor(RGB(255, 255, 255));
	memDC.DrawText(_T("SCORE"), &scoreRect, DT_SINGLELINE | DT_CENTER | DT_TOP);
	CString score2draw;
	score2draw.Format(_T("%d"), Score);
	memDC.DrawText(score2draw, &scoreRect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	memDC.SelectObject(pOldFont);
	scoreFont.DeleteObject();

	/*---绘制Game Over提示---*/
	if (isFail){
		CFont noticeFont;
		noticeFont.CreateFont(80, 32, 0, 0,
			FW_EXTRABOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
		pOldFont = memDC.SelectObject(&noticeFont);
		memDC.SetTextColor(RGB(119, 110, 101));
		memDC.DrawText(_T("Game Over!"), &boardRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		memDC.SelectObject(pOldFont);
		noticeFont.DeleteObject();
	}

	//将内存DC中的图像拷贝到前台
	pDC->BitBlt(boardRect.left, boardRect.top, boardRect.right - boardRect.left, boardRect.bottom - boardRect.top,
		&memDC, boardRect.left, boardRect.top, SRCCOPY);
	pDC->BitBlt(scoreRect.left, scoreRect.top, scoreRect.right - scoreRect.left, scoreRect.bottom - scoreRect.top, 
		&memDC, scoreRect.left, scoreRect.top, SRCCOPY);

	//释放内存
	memDC.SelectObject(pOldBit);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
	ReleaseDC(pDC);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy2048Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//捕捉键盘按键
BOOL CMy2048Dlg::PreTranslateMessage(MSG * pMsg){
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam){
		case VK_LEFT:
			MoveAlong(LEFT);break;
		case VK_RIGHT:
			MoveAlong(RIGHT);break;
		case VK_UP:
			MoveAlong(UP);break;
		case VK_DOWN:
			MoveAlong(DOWN);break;
		default:break;
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//新局初始化
void CMy2048Dlg::OnBnClickedRestart(){
	memset(Matrix, 0, sizeof(Matrix));//矩阵清空	
	Score = 0;
	isUndoValid = false;
	GetDlgItem(IDC_UNDO)->EnableWindow(FALSE);//禁用Undo按钮
	GetDlgItem(IDC_RESTART)->SetFocus();//设置焦点，防止捕捉键盘消息失败
	isFail = false;
	putNewTile();putNewTile();//放置两个初始块
}

//撤销操作
void CMy2048Dlg::OnBnClickedUndo() {
	if (isUndoValid) {
		memmove(Matrix, Last, sizeof(int) * 16);//恢复矩阵
		isUndoValid = false;//禁止撤销
		GetDlgItem(IDC_UNDO)->EnableWindow(FALSE);//禁用Undo按钮
		GetDlgItem(IDC_RESTART)->SetFocus();//设置焦点，防止捕捉键盘失败
		isFail = false;
	}
}

//存档
void CMy2048Dlg::OnBnClickedSave() {
	CString FileName = CTime::GetCurrentTime().Format("2048_%Y%m%d%H%M%S");//以当前时间为默认文件名
	CString Filter = _T("2048游戏进度存档文件(*.sav)|*.sav||");
	CString FilePathName;
	CFileDialog dlg(FALSE, _T("sav"), FileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, Filter);//Save as对话框
	if (dlg.DoModal() == IDOK)FilePathName = dlg.GetPathName();

	//二进制文件写入
	ofstream fileOutBin(FilePathName, ios::out | ios::binary);
	fileOutBin.write(reinterpret_cast<char *>(&Matrix), sizeof(Matrix));
	fileOutBin.write(reinterpret_cast<char *>(&Last), sizeof(Last));
	fileOutBin.write(reinterpret_cast<char *>(&Score), sizeof(Score));
	fileOutBin.write(reinterpret_cast<char *>(&isUndoValid), sizeof(isUndoValid));
	fileOutBin.write(reinterpret_cast<char *>(&isFail), sizeof(isFail));
	fileOutBin.close();
}

//读档
void CMy2048Dlg::OnBnClickedLoad() {
	CString Filter = _T("2048游戏进度存档文件(*.sav)|*.sav||");
	CString FilePathName;
	CFileDialog dlg(TRUE, _T("sav"), NULL, OFN_HIDEREADONLY | OFN_READONLY, Filter);//Open对话框
	if (dlg.DoModal() == IDOK)FilePathName = dlg.GetPathName();

	//二进制文件读出
	ifstream fileInBin(FilePathName, ios::in | ios::binary);
	fileInBin.read(reinterpret_cast<char *>(&Matrix), sizeof(Matrix));
	fileInBin.read(reinterpret_cast<char *>(&Last), sizeof(Last));
	fileInBin.read(reinterpret_cast<char *>(&Score), sizeof(Score));
	fileInBin.read(reinterpret_cast<char *>(&isUndoValid), sizeof(isUndoValid));
	fileInBin.read(reinterpret_cast<char *>(&isFail), sizeof(isFail));
	fileInBin.close();
	if (isUndoValid)GetDlgItem(IDC_UNDO)->EnableWindow(TRUE);//若为可撤销状态，启用Undo按钮
}

//退出确认，避免未存档误关游戏
void CMy2048Dlg::OnClose()
{
	if (IDNO == ::MessageBox(this->m_hWnd, _T("Please be aware that you can save your game \
and load it back at any time.\nSure to quit?"), _T("Quit Game"), MB_ICONQUESTION | MB_YESNO))return;
	CDialog::OnClose();
}

//向矩阵中放置一个新块（2或4）
void CMy2048Dlg::putNewTile(){
	//记录所有空格的位置
	deque<int> posBlank;
	for (int i = 0;i != 16;++i)
		if (0 == Matrix[0][i])posBlank.push_back(i);
	if (posBlank.empty()) return;//无空返回
	Matrix[0][posBlank[rand() % posBlank.size()]] = rand() % 100 > 50 ? 2 : 1;//随机生成块
}

//沿一个方向移动，参数为移动方向
void CMy2048Dlg::MoveAlong(ARROW direction) {
	if (isFail) return;//游戏已失败，不响应
	
	//捕捉移动前状态
	int capture[4][4];
	memcpy(capture, Matrix, sizeof(int) * 16);
	//for (int i = 0;i != 4;++i)for (int j = 0;j != 4;++j)capture[i][j] = Matrix[i][j];

	deque<int>temp;//用于向一个方向压实数字块，顺序用push_back，逆序用push_front，因此使用deque
	
	switch (direction) {
	case LEFT:
		for (int row = 0;row != 4;++row) {
			for (int col = 0;col < 3;++col) {//向左压，从左向右合并
				if (Matrix[col][row] == 0)continue;//该格为空，跳过
				for (int pos = col + 1;pos < 4;++pos) {
					if (Matrix[pos][row] == 0)continue;//该格为空，跳过
					if (Matrix[pos][row] == Matrix[col][row]) {//合并相同格
						Matrix[pos][row] = 0;//右格清空
						Matrix[col][row]++;//左格指数增加1
						Score += tile[Matrix[col][row]].num;//加分
						col=pos;
						break;
					}
					break;//遇到不相同的块，退出
				}
			}
			//向左压实
			temp.clear();
			for (int col = 0;col != 4;++col) if (Matrix[col][row] != 0)temp.push_back(Matrix[col][row]);//取非0数字
			for (int i = 0;i != temp.size();++i)Matrix[i][row] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[i][row] = 0;
		}
		break;
	case RIGHT:
		for (int row = 0;row != 4;++row) {
			for (int col = 3;col > 0;--col) {//向右压，从右向左合并
				if (Matrix[col][row] == 0)continue;//该格为空，跳过
				for (int pos = col - 1;pos > -1;--pos) {
					if (Matrix[pos][row] == 0)continue;//该格为空，跳过
					if (Matrix[pos][row] == Matrix[col][row]) {//合并相同格
						Matrix[pos][row] = 0;//左格清空
						Matrix[col][row]++;//右格指数增加1
						Score += tile[Matrix[col][row]].num;
						col=pos;
						break;
					}
					break;
				}
			}
			//向右压实
			temp.clear();
			for (int col = 0;col != 4;++col) if (Matrix[col][row] != 0)temp.push_front(Matrix[col][row]);//取非0数字
			for (int i = 0;i != temp.size();++i)Matrix[3 - i][row] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[3 - i][row] = 0;
		}
		break;
	case UP:
		for (int col = 0;col != 4;++col) {
			for (int row = 0;row < 3;++row) {//向上压，从上向下合并
				if (Matrix[col][row] == 0)continue;//该格为空，跳过
				for (int pos = row + 1;pos < 4;++pos) {
					if (Matrix[col][pos] == 0)continue;//该格为空，跳过
					if (Matrix[col][pos] == Matrix[col][row]) {//合并相同格
						Matrix[col][pos] = 0;//下格清空
						Matrix[col][row]++;//上格指数加1
						Score += tile[Matrix[col][row]].num;
						row=pos;
						break;
					}
					break;
				}
			}
			//向上压实
			temp.clear();
			for (int row = 0;row != 4;++row) if (Matrix[col][row] != 0)temp.push_back(Matrix[col][row]);//取非0数字
			for (int i = 0;i != temp.size();++i)Matrix[col][i] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[col][i] = 0;
		}
		break;
	case DOWN:
		for (int col = 0;col < 4;++col) {
			for (int row = 3;row > 0;--row) {//向下压，从下向上合并
				if (Matrix[col][row] == 0)continue;//该格为空，跳过
				for (int pos = row - 1;pos > -1;--pos) {
					if (Matrix[col][pos] == 0)continue;//该格为空，跳过
					if (Matrix[col][pos] == Matrix[col][row]) {//合并相同格
						Matrix[col][pos] = 0;//上格清空
						Matrix[col][row]++;//下格指数加1
						Score += tile[Matrix[col][row]].num;
						row=pos;
						break;
					}
					break;
				}
			}
			//向下压实
			temp.clear();
			for (int row = 0;row != 4;++row) if (Matrix[col][row] != 0)temp.push_front(Matrix[col][row]);//取非0数字
			for (int i = 0;i != temp.size();++i)Matrix[col][3 - i] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[col][3 - i] = 0;
		}
		break;
	default:break;
	}

	//检查游戏矩阵是否发生改变
	bool changed = false;
	for (int i = 0;i != 4;++i) {
		for (int j = 0;j != 4;++j) {
			if (capture[i][j] != Matrix[i][j]) { changed = true; break; }
		}
		if (changed)break;
	}
	if (changed) {
		putNewTile();//发生改变则是有效操作，将增加一个新块
		memmove(Last,capture,sizeof(int)*16);//将capture保存为Last状态
		
		if (!isUndoValid) {//已有Last状态，可以允许撤销了
			isUndoValid = true;
			GetDlgItem(IDC_UNDO)->EnableWindow(TRUE);
		}
	}

	//判断移动后游戏是否失败
	for (int i = 0; i != 16; ++i)
			if (Matrix[0][i] == 0)return;//有空格		
	for (int col = 0; col != 3; ++col)
		for (int row = 0; row != 4; ++row)
			if (Matrix[col][row] == Matrix[col + 1][row])return;//横向有相邻相同块		
	for (int col = 0; col != 4; ++col)
		for (int row = 0; row != 3; ++row)
			if (Matrix[col][row] == Matrix[col][row + 1])return;//纵向有相邻相同块	
	isFail = true;
}
