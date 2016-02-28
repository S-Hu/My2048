
// My2048Dlg.cpp : ʵ���ļ�
//������ѧ��C++���Գ�����ơ��γ���ҵ
//1400015962 ��˳�

#include "stdafx.h"
#include "My2048.h"
#include "My2048Dlg.h"
#include "afxdialogex.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2048Dlg �Ի���

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


// CMy2048Dlg ��Ϣ�������

BOOL CMy2048Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	srand((unsigned)time(NULL));//�������������
	OnBnClickedRestart();//�¾ֳ�ʼ��
	SetTimer(1, 100, NULL);//���ü�ʱ������ʱˢ�»�ͼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy2048Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		//��䱳��ɫ
		dc.FillSolidRect(rect, RGB(250, 248, 239));
		//���Ʊ���
		CFont titleFont;//��������
		titleFont.CreateFont(80, 40, 0, 0,
			FW_BLACK, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
		CFont *pOldFont = dc.SelectObject(&titleFont);//ѡ������
		dc.SetTextColor(RGB(119, 110, 101));//������ɫ
		RECT titleRect = { 40,10,230,80 };//�����������
		dc.DrawText(_T("2048"), &titleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);//�����ı�
		//�ͷ��ڴ�
		dc.SelectObject(pOldFont);
		titleFont.DeleteObject();

		CDialog::OnPaint();
	}
}

//��ʱ���¼�����ʱˢ�»��ƴ���
void CMy2048Dlg::OnTimer(UINT_PTR nIDEvent){
	//��ͼ��ʼ����ʹ��˫�����ͼ���������ͼ����˸��
	RECT rect;
	GetClientRect(&rect);
	CDC *pDC = GetDC();
	CDC memDC;//������ʾ�豸����
	CBitmap memBitmap;//����λͼ����
	memDC.CreateCompatibleDC(pDC);//��������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	memBitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);//��������Ļ��ʾ���ݵ�λͼ
	CBitmap *pOldBit = memDC.SelectObject(&memBitmap);//��λͼѡ���ڴ�DC
	memDC.SetBkMode(TRANSPARENT);//�����ַ�������Ϊ͸��
	CFont *pOldFont;

	//����Ŀ��Ϊ100�������ļ��Ϊ10

	/*---��������---*/
	RECT boardRect = { 40, 150, 490, 600 };
	memDC.FillSolidRect(&boardRect, RGB(187, 173, 160));

	/*---���ƴſ�---*/
	for (int col = 0; col < 4; ++col)
		for (int row = 0; row < 4; ++row) {
			RECT tileRect = { 50 + col * 110, 160 + row * 110, 150 + col * 110, 260 + row * 110};//��ǰ��λ��
			const TileTraits &thisTile = tile[Matrix[col][row]];//ȡ��ǰ������
			memDC.FillSolidRect(&tileRect, thisTile.backColor);//��䱳��
			//��������
			if (thisTile.num != 0) {//0Ϊ�ո񣬲���
				CFont numFont;
				numFont.CreateFont(thisTile.fontHeight, thisTile.fontWidth, 0, 0,
					FW_BLACK, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
					CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
				pOldFont = memDC.SelectObject(&numFont);
				CString num2draw;
				num2draw.Format(_T("%d"), thisTile.num);//����תCString
				memDC.SetTextColor(thisTile.frontColor);
				memDC.DrawText(num2draw, &tileRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				memDC.SelectObject(pOldFont);
				numFont.DeleteObject();
			}
		}

	/*---���Ƶ÷�---*/
	//����
	RECT scoreRect = { boardRect.right - 80, 20, boardRect.right, 70 };
	memDC.FillSolidRect(&scoreRect, RGB(187, 173, 160));
	//����
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

	/*---����Game Over��ʾ---*/
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

	//���ڴ�DC�е�ͼ�񿽱���ǰ̨
	pDC->BitBlt(boardRect.left, boardRect.top, boardRect.right - boardRect.left, boardRect.bottom - boardRect.top,
		&memDC, boardRect.left, boardRect.top, SRCCOPY);
	pDC->BitBlt(scoreRect.left, scoreRect.top, scoreRect.right - scoreRect.left, scoreRect.bottom - scoreRect.top, 
		&memDC, scoreRect.left, scoreRect.top, SRCCOPY);

	//�ͷ��ڴ�
	memDC.SelectObject(pOldBit);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
	ReleaseDC(pDC);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy2048Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��׽���̰���
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

//�¾ֳ�ʼ��
void CMy2048Dlg::OnBnClickedRestart(){
	memset(Matrix, 0, sizeof(Matrix));//�������	
	Score = 0;
	isUndoValid = false;
	GetDlgItem(IDC_UNDO)->EnableWindow(FALSE);//����Undo��ť
	GetDlgItem(IDC_RESTART)->SetFocus();//���ý��㣬��ֹ��׽������Ϣʧ��
	isFail = false;
	putNewTile();putNewTile();//����������ʼ��
}

//��������
void CMy2048Dlg::OnBnClickedUndo() {
	if (isUndoValid) {
		memmove(Matrix, Last, sizeof(int) * 16);//�ָ�����
		isUndoValid = false;//��ֹ����
		GetDlgItem(IDC_UNDO)->EnableWindow(FALSE);//����Undo��ť
		GetDlgItem(IDC_RESTART)->SetFocus();//���ý��㣬��ֹ��׽����ʧ��
		isFail = false;
	}
}

//�浵
void CMy2048Dlg::OnBnClickedSave() {
	CString FileName = CTime::GetCurrentTime().Format("2048_%Y%m%d%H%M%S");//�Ե�ǰʱ��ΪĬ���ļ���
	CString Filter = _T("2048��Ϸ���ȴ浵�ļ�(*.sav)|*.sav||");
	CString FilePathName;
	CFileDialog dlg(FALSE, _T("sav"), FileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, Filter);//Save as�Ի���
	if (dlg.DoModal() == IDOK)FilePathName = dlg.GetPathName();

	//�������ļ�д��
	ofstream fileOutBin(FilePathName, ios::out | ios::binary);
	fileOutBin.write(reinterpret_cast<char *>(&Matrix), sizeof(Matrix));
	fileOutBin.write(reinterpret_cast<char *>(&Last), sizeof(Last));
	fileOutBin.write(reinterpret_cast<char *>(&Score), sizeof(Score));
	fileOutBin.write(reinterpret_cast<char *>(&isUndoValid), sizeof(isUndoValid));
	fileOutBin.write(reinterpret_cast<char *>(&isFail), sizeof(isFail));
	fileOutBin.close();
}

//����
void CMy2048Dlg::OnBnClickedLoad() {
	CString Filter = _T("2048��Ϸ���ȴ浵�ļ�(*.sav)|*.sav||");
	CString FilePathName;
	CFileDialog dlg(TRUE, _T("sav"), NULL, OFN_HIDEREADONLY | OFN_READONLY, Filter);//Open�Ի���
	if (dlg.DoModal() == IDOK)FilePathName = dlg.GetPathName();

	//�������ļ�����
	ifstream fileInBin(FilePathName, ios::in | ios::binary);
	fileInBin.read(reinterpret_cast<char *>(&Matrix), sizeof(Matrix));
	fileInBin.read(reinterpret_cast<char *>(&Last), sizeof(Last));
	fileInBin.read(reinterpret_cast<char *>(&Score), sizeof(Score));
	fileInBin.read(reinterpret_cast<char *>(&isUndoValid), sizeof(isUndoValid));
	fileInBin.read(reinterpret_cast<char *>(&isFail), sizeof(isFail));
	fileInBin.close();
	if (isUndoValid)GetDlgItem(IDC_UNDO)->EnableWindow(TRUE);//��Ϊ�ɳ���״̬������Undo��ť
}

//�˳�ȷ�ϣ�����δ�浵�����Ϸ
void CMy2048Dlg::OnClose()
{
	if (IDNO == ::MessageBox(this->m_hWnd, _T("Please be aware that you can save your game \
and load it back at any time.\nSure to quit?"), _T("Quit Game"), MB_ICONQUESTION | MB_YESNO))return;
	CDialog::OnClose();
}

//������з���һ���¿飨2��4��
void CMy2048Dlg::putNewTile(){
	//��¼���пո��λ��
	deque<int> posBlank;
	for (int i = 0;i != 16;++i)
		if (0 == Matrix[0][i])posBlank.push_back(i);
	if (posBlank.empty()) return;//�޿շ���
	Matrix[0][posBlank[rand() % posBlank.size()]] = rand() % 100 > 50 ? 2 : 1;//������ɿ�
}

//��һ�������ƶ�������Ϊ�ƶ�����
void CMy2048Dlg::MoveAlong(ARROW direction) {
	if (isFail) return;//��Ϸ��ʧ�ܣ�����Ӧ
	
	//��׽�ƶ�ǰ״̬
	int capture[4][4];
	memcpy(capture, Matrix, sizeof(int) * 16);
	//for (int i = 0;i != 4;++i)for (int j = 0;j != 4;++j)capture[i][j] = Matrix[i][j];

	deque<int>temp;//������һ������ѹʵ���ֿ飬˳����push_back��������push_front�����ʹ��deque
	
	switch (direction) {
	case LEFT:
		for (int row = 0;row != 4;++row) {
			for (int col = 0;col < 3;++col) {//����ѹ���������Һϲ�
				if (Matrix[col][row] == 0)continue;//�ø�Ϊ�գ�����
				for (int pos = col + 1;pos < 4;++pos) {
					if (Matrix[pos][row] == 0)continue;//�ø�Ϊ�գ�����
					if (Matrix[pos][row] == Matrix[col][row]) {//�ϲ���ͬ��
						Matrix[pos][row] = 0;//�Ҹ����
						Matrix[col][row]++;//���ָ������1
						Score += tile[Matrix[col][row]].num;//�ӷ�
						col=pos;
						break;
					}
					break;//��������ͬ�Ŀ飬�˳�
				}
			}
			//����ѹʵ
			temp.clear();
			for (int col = 0;col != 4;++col) if (Matrix[col][row] != 0)temp.push_back(Matrix[col][row]);//ȡ��0����
			for (int i = 0;i != temp.size();++i)Matrix[i][row] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[i][row] = 0;
		}
		break;
	case RIGHT:
		for (int row = 0;row != 4;++row) {
			for (int col = 3;col > 0;--col) {//����ѹ����������ϲ�
				if (Matrix[col][row] == 0)continue;//�ø�Ϊ�գ�����
				for (int pos = col - 1;pos > -1;--pos) {
					if (Matrix[pos][row] == 0)continue;//�ø�Ϊ�գ�����
					if (Matrix[pos][row] == Matrix[col][row]) {//�ϲ���ͬ��
						Matrix[pos][row] = 0;//������
						Matrix[col][row]++;//�Ҹ�ָ������1
						Score += tile[Matrix[col][row]].num;
						col=pos;
						break;
					}
					break;
				}
			}
			//����ѹʵ
			temp.clear();
			for (int col = 0;col != 4;++col) if (Matrix[col][row] != 0)temp.push_front(Matrix[col][row]);//ȡ��0����
			for (int i = 0;i != temp.size();++i)Matrix[3 - i][row] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[3 - i][row] = 0;
		}
		break;
	case UP:
		for (int col = 0;col != 4;++col) {
			for (int row = 0;row < 3;++row) {//����ѹ���������ºϲ�
				if (Matrix[col][row] == 0)continue;//�ø�Ϊ�գ�����
				for (int pos = row + 1;pos < 4;++pos) {
					if (Matrix[col][pos] == 0)continue;//�ø�Ϊ�գ�����
					if (Matrix[col][pos] == Matrix[col][row]) {//�ϲ���ͬ��
						Matrix[col][pos] = 0;//�¸����
						Matrix[col][row]++;//�ϸ�ָ����1
						Score += tile[Matrix[col][row]].num;
						row=pos;
						break;
					}
					break;
				}
			}
			//����ѹʵ
			temp.clear();
			for (int row = 0;row != 4;++row) if (Matrix[col][row] != 0)temp.push_back(Matrix[col][row]);//ȡ��0����
			for (int i = 0;i != temp.size();++i)Matrix[col][i] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[col][i] = 0;
		}
		break;
	case DOWN:
		for (int col = 0;col < 4;++col) {
			for (int row = 3;row > 0;--row) {//����ѹ���������Ϻϲ�
				if (Matrix[col][row] == 0)continue;//�ø�Ϊ�գ�����
				for (int pos = row - 1;pos > -1;--pos) {
					if (Matrix[col][pos] == 0)continue;//�ø�Ϊ�գ�����
					if (Matrix[col][pos] == Matrix[col][row]) {//�ϲ���ͬ��
						Matrix[col][pos] = 0;//�ϸ����
						Matrix[col][row]++;//�¸�ָ����1
						Score += tile[Matrix[col][row]].num;
						row=pos;
						break;
					}
					break;
				}
			}
			//����ѹʵ
			temp.clear();
			for (int row = 0;row != 4;++row) if (Matrix[col][row] != 0)temp.push_front(Matrix[col][row]);//ȡ��0����
			for (int i = 0;i != temp.size();++i)Matrix[col][3 - i] = temp[i];
			for (int i = temp.size();i != 4;++i)Matrix[col][3 - i] = 0;
		}
		break;
	default:break;
	}

	//�����Ϸ�����Ƿ����ı�
	bool changed = false;
	for (int i = 0;i != 4;++i) {
		for (int j = 0;j != 4;++j) {
			if (capture[i][j] != Matrix[i][j]) { changed = true; break; }
		}
		if (changed)break;
	}
	if (changed) {
		putNewTile();//�����ı�������Ч������������һ���¿�
		memmove(Last,capture,sizeof(int)*16);//��capture����ΪLast״̬
		
		if (!isUndoValid) {//����Last״̬��������������
			isUndoValid = true;
			GetDlgItem(IDC_UNDO)->EnableWindow(TRUE);
		}
	}

	//�ж��ƶ�����Ϸ�Ƿ�ʧ��
	for (int i = 0; i != 16; ++i)
			if (Matrix[0][i] == 0)return;//�пո�		
	for (int col = 0; col != 3; ++col)
		for (int row = 0; row != 4; ++row)
			if (Matrix[col][row] == Matrix[col + 1][row])return;//������������ͬ��		
	for (int col = 0; col != 4; ++col)
		for (int row = 0; row != 3; ++row)
			if (Matrix[col][row] == Matrix[col][row + 1])return;//������������ͬ��	
	isFail = true;
}
