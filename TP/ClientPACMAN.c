// ============================================================================
// Início do programa
// ============================================================================
#include <windows.h>
#include <stdio.h>		// Para chamada à função "sprintf" 
#include <string.h>
#include <wingdi.h>		// Para chamada a algumas funções de gráficos
						

#include "resource.h"
#define TAM 15
#define LARGURA 600
#define ALTURA 400
char szProgName[] = "PACMAN",dir='R';
WPARAM direita,esquerda,cima,baixo;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInstance;				// Instance (variável global para manter hInst)
HBITMAP hbit;	// hanlde para um bitmap
HBITMAP food,bad,parede,pac;	
int x=-1, y,colocar=0,pontos=0, joga=0,vidas=3, zero=0;

int mapa[14][24]={4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
				  4,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,4,
				  4,0,4,4,4,4,0,4,4,4,0,4,0,4,4,4,0,4,4,4,4,4,0,4,
				  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
				  4,0,4,4,4,4,0,4,4,4,4,4,4,4,4,4,0,4,4,4,4,4,0,4,
				  4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,4,
				  4,4,4,0,4,4,0,0,0,4,0,0,0,4,0,0,0,4,4,4,4,4,0,4,
				  4,0,0,0,0,0,4,4,0,4,3,3,3,4,0,4,4,4,0,0,0,0,0,4,
				  4,0,4,4,4,0,0,0,0,4,4,4,4,4,0,0,0,0,0,4,4,4,0,4,
				  4,0,0,0,4,4,0,4,4,4,0,0,0,0,0,4,4,0,4,4,0,0,0,4,
				  4,0,4,0,0,4,0,0,0,0,0,4,0,4,0,0,0,0,4,0,0,4,0,4,
				  4,0,4,4,0,4,0,4,4,4,4,4,0,4,4,4,4,0,4,0,4,4,0,4,
				  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
				  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
int mfood[14][24];
// ============================================================================
// WinMain()
// Parâmetros:
//   hInst:		handle da instância deste programa 
//   hPrevInst: Sempre NULL para o NT (era usado no Windows 3.1)
//   lpCmdLine: Ponteiro para uma string ASCIIZ de parâmetros para o programa
//   nCmdShow:  Modo de exibição da janela 


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) 
	{
	HWND hWnd;			// handler da janela (a gerar por CreateWindow())
	MSG lpMsg;			// Estrutura das mensagens
	WNDCLASSEX wcApp;	// Estrutura que define a classe da janela

// ============================================================================
	hInstance=hInst;	// Incializar global hInstance

// ============================================================================
// Características da janela "wcApp" 
	wcApp.cbSize = sizeof(WNDCLASSEX);	// Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;			// Instância da janela actualmente exibida 
										// ("hInst" é parâmetro de WinMain)
	wcApp.lpszClassName = szProgName;	// Nome da janela 
	wcApp.lpfnWndProc = WndProc;		// Endereço da função de processamento da janela 

// Fazer redraw hor. e ver.
	wcApp.style = CS_HREDRAW | CS_VREDRAW ;
	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// ícon normal=Aplicação do Windows
	
//==============================================================================
// Ícon definido pelo programador

	wcApp.hIconSm = LoadIcon(hInst, (LPCTSTR) IDI_ICON1);	// ícon definido pelo
															// programador		
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);			// rato = "seta"
//==============================================================================
// Menu do programa
	wcApp.lpszMenuName = (LPCTSTR) IDR_MENU1;		// Menu da janela		
	
	wcApp.cbClsExtra = 0;							// Livre, para uso particular
	wcApp.cbWndExtra = 0;	// Livre, para uso particular
	wcApp.hbrBackground = GetStockObject(BLACK_BRUSH);	// Fundo branco

// ============================================================================
// Registar a classe "wcApp" 
	if (!RegisterClassEx(&wcApp))
		return(0);

// ============================================================================
// Criar a janela
	hWnd = CreateWindow(
		szProgName,				// Nome da janela e/ou programa
		"PACMAN",	// Título da janela
		WS_OVERLAPPEDWINDOW,	// Estilo da janela 
		30,			// Posição x 
		30,			// Posição y 
		LARGURA+16,			// Largura 
		ALTURA+8+20,			// Altura 
		(HWND) HWND_DESKTOP,	// handle da janela pai (HWND_DESKTOP para 1ª)
		(HMENU) NULL,			// handle do menu (se tiver menu)
		(HANDLE) hInst,			// handle da instância actual (vem de WinMain())
		(LPSTR) NULL);			// Não há parâmetros adicionais 

// ============================================================================
// Mostrar a janela
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela
								// "nCmdShow"= modo, parâmetro de WinMain()
	UpdateWindow(hWnd);			// Refrescar a janela 

// ============================================================================
// Loop de Mensagens
// Parâmetros de "getMessage":
//  1)	"&lpMsg"=Endereço de uma estrutura do tipo MSG ("MSG lpMsg" ja foi 
//		declarada no início de WinMain()):
	esquerda=VK_LEFT;
	direita=VK_RIGHT;
	baixo=VK_DOWN;
	cima=VK_UP;
	while (GetMessage(&lpMsg,NULL,0,0)) {	
			TranslateMessage(&lpMsg);		// Pré-processamento da mensagem
			DispatchMessage(&lpMsg);		// Enviar a mensagem traduzida de volta
											// ao Windows
	}

// ============================================================================
// Fim do programa
	return((int)lpMsg.wParam);	// Status = Parâmetro "wParam" da estrutura "lpMsg"
}
void controicomida(){
	
	int i,j;
	for(j=0;j<=14;j++)
		for(i=0;i<=24;i++){
			if(mapa[j][i]==0){
				zero++;
				mfood[j][i]=1;
			}else
				mfood[j][i]=0;
		}
}
void desenhapacman(HWND hWnd){
	HDC hdc,memdc;
	hdc=GetDC(hWnd);					
		
// Carregar o BitMap
	switch(dir){
	case 'R':hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));	break;
	case 'L':hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));  break;
	case 'U':hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));  break;
	case 'D':hbit=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));  break;
	}
// Criar em meória uma "Imagem" compatível com a janela actual
	memdc=CreateCompatibleDC(hdc);	
// Colocar o bitmap nesta zona de memória
	SelectObject(memdc, hbit);		
// Copiar para o écran coordenadas (200,200) (200,200) o conteúdo "apontado" pelo handler
// "memdc" a partir da posição (0,0). Trata-se de uma cópia simples "SRCCOPY"
	BitBlt(hdc, x*25,y*25, 25,25, memdc, 0,0, SRCCOPY);
	DeleteDC(memdc);
	ReleaseDC(hWnd,hdc);
	
}

void desenhamapa(HWND hWnd){
	HDC hdc,memdc;				
	int i,j;
	hdc=GetDC(hWnd);
	// Carregar o BitMap
	food=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));	
	bad=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5)); 
	pac=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	parede=LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
	// Criar em meória uma "Imagem" compatível com a janela actual
	memdc=CreateCompatibleDC(hdc);
	for(j=0;j<14;j++)
		for(i=0;i<24;i++)
			if(mapa[j][i]>0){
				switch(mapa[j][i]){
					case 2:hbit=pac; break;
					case 3:hbit=bad; break;
					case 4:hbit=parede; break;
					default:break;
				}
				SelectObject(memdc, hbit);
				BitBlt(hdc, i*25,j*25, 25,25, memdc, 0,0, SRCCOPY);
			
			}else{
				if(mfood[j][i]==1){
					hbit=food;
					SelectObject(memdc, hbit);
					BitBlt(hdc, i*25,j*25, 25,25, memdc, 0,0, SRCCOPY);
				
				}
			}

	DeleteDC(memdc);
	ReleaseDC(hWnd,hdc);
}
void tratacomando( WPARAM wParam,HWND hWnd){
		if (wParam==cima) {
			if(y!=0&&mapa[y-1][x]<2)
				y-=1;	
			dir='U';
		}
		if ((char) wParam==VK_DOWN) {
			if(y!=ALTURA/25-3&&mapa[y+1][x]<2)
				y+=1;
			dir='D';
		}	
		if ((char) wParam==VK_LEFT) {
			if(x!=0&&mapa[y][x-1]<2)
				x-=1;
			dir='L';
		}
		if ((char) wParam==VK_RIGHT) {
			if(x!=LARGURA/25-1&&mapa[y][x+1]<2)
				x+=1;
			dir='R';
		}
		if(colocar==1){
			if(mfood[y][x]==1){
				pontos++;
				zero--;
				mfood[y][x]=0;
			}
			if(vidas==zero){
				//perdeu
				joga=0;
				x=-1;
				colocar=0;
				
			}
			if(zero==0){
				//FIM
				joga=0;
				x=-1;
				colocar=0;
				MessageBox(hWnd, "GANHOU", "INFO", MB_OK);
			}
		}
}
void desenhainfo(HWND hWnd){
	char str[TAM];
	HDC hdc;		
	hdc=GetDC(hWnd);
	sprintf_s(str,TAM,"vidas: %d ",vidas);
	TextOut(hdc,10,352,str,(int)strlen(str));
	sprintf_s(str,TAM,"Score: %d ",pontos);
	TextOut(hdc,70,352,str,(int)strlen(str));

}

/*BOOL CALLBACK tratadir(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	
	switch(messg) {
		case WM_KEYDOWN:
			direita=wParam;
			EndDialog(hWnd,1);
			return 1;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				return 1;
			case IDCANCEL:
				direita=VK_RIGHT;
				EndDialog(hWnd,1);
				return 1;
		}
		default:return(DefWindowProc(hWnd,messg,wParam,lParam));
		break;
	}
	return 0;
}
BOOL CALLBACK trataesq(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	int i;
	switch(messg) {
		case WM_KEYDOWN:
			
			esquerda=wParam;
			EndDialog(hWnd,1);
			return 1;
		case WM_COMMAND:
		switch(LOWORD(wParam)) {
				return 1;
			case IDCANCEL:
				esquerda=VK_LEFT;
				EndDialog(hWnd,1);
				return 1;
		}
		default:break;
	}
	return 0;
}
BOOL CALLBACK tratacim(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	int i;
	switch(messg) {
		case WM_KEYDOWN:
			cima=wParam;
			EndDialog(hWnd,1);
			
			return 1;
		case WM_COMMAND:
		switch(LOWORD(wParam)) {
				return 1;
			case IDCANCEL:
				cima=VK_UP;
				EndDialog(hWnd,1);
				return 1;
		}
		default:break;
	}
	return 0;
}
BOOL CALLBACK tratabai(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	int i;
	switch(messg) {
		case WM_KEYDOWN:

			baixo=wParam;
			EndDialog(hWnd,1);
			return 1;
		case WM_COMMAND:
		switch(LOWORD(wParam)) {
				return 1;
			case IDCANCEL:
				baixo=VK_DOWN;
				EndDialog(hWnd,1);
				return 1;
		}
		default:break;
	}
	return 0;
}
BOOL CALLBACK trata_teclas(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	int i;
	HWND hdlg;
	

	switch(messg) {
		case WM_COMMAND:
		switch(LOWORD(wParam)) {
			case IDC_BUTTON1:hdlg=CreateDialog(hInstance,(LPCSTR)IDD_DIALOG2,hWnd,(DLGPROC)tratacim);ShowWindow(hdlg,SW_SHOW);break;//cima
			case IDC_BUTTON2:DialogBox(hInstance,(LPCSTR)IDD_DIALOG2,hWnd,(DLGPROC)tratabai);break;//baixo
			case IDC_BUTTON3:DialogBox(hInstance,(LPCSTR)IDD_DIALOG2,hWnd,(DLGPROC)tratadir);break;//direita
			case IDC_BUTTON4:DialogBox(hInstance,(LPCSTR)IDD_DIALOG2,hWnd,(DLGPROC)trataesq);break;//esquerda
			case IDOK:
			// bla bla set teclas
				EndDialog(hWnd,1);
				return 1;
			case IDCANCEL:
				esquerda=VK_LEFT;
				direita=VK_RIGHT;
				baixo=VK_DOWN;
				cima=VK_UP;
				EndDialog(hWnd,1);
				return 1;
			
		}
		default:break;
	}
	return 0;
}
*/

// ============================================================================
// FUNÇÂO DE PROCESSAMENTO DA JANELA
// ============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) 
{
	
	HDC hdc;						// handler para um Device Context
	
	static int contador=1;	// Contador de entradas em WM_PAINT
	PAINTSTRUCT PtStc;
	int resposta;					// Resposta a MessageBox
// Processamento das mensagens
	switch (messg) {
//==============================================================================
// Resposta a opções do Menu da janela

		//movimentos
	case WM_KEYDOWN:
		tratacomando(wParam,hWnd);
		InvalidateRect(hWnd, NULL, 1);
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&PtStc);
		if(joga==1){
			if(x!=-1)
				desenhapacman(hWnd);
			desenhamapa(hWnd);
			EndPaint(hWnd,&PtStc);
			desenhainfo(hWnd);
			
		}else
			TextOut(hdc,270,160,"PACMAN",6);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_JOGAR_NOVOJOGO:joga=1;InvalidateRect(hWnd, NULL, 1);break;
		case ID_JOGAR_JOGOEXISTENTE:break;
		case ID_CONFIGURAR_PONTOS:break;
		case ID_CONFIGURAR_TECLAS:// DialogBox(hInstance, (LPCTSTR) IDD_DIALOG1, hWnd, (DLGPROC)trata_teclas);
			break;
		case ID_VIEW_SCORE:break;
		case ID_VIEW_BEST:break;
		case ID_EXIT:
			resposta=MessageBox(hWnd,"Terminar o Programa?", "Fim", MB_YESNO|MB_ICONQUESTION);
			if (resposta==IDYES)
				PostQuitMessage(0);				// Se YES, terminar programa
			break;
		}
		

//==============================================================================
// Marcar a posição do rato
	case WM_LBUTTONDOWN:		// Botão esquerdo
		if(colocar==0&&joga==1){
			x=(int)(LOWORD(lParam)/25);
			y=(int)(HIWORD(lParam)/25);
			if(x<23&&y<13){
				if(mapa[y][x]==0){
					InvalidateRect(hWnd, NULL, 1);
					colocar=1;
					controicomida();
					mfood[y][x]=0;
					zero--;
				}
			}else
				x=-1;
		}
		break;

//==============================================================================
// Detectar movimento da janela
	case WM_MOVE:						// Detectar que a janela se moveu
		InvalidateRect(hWnd, NULL, 1);	// Gerar WM_PAINT para actualizar 
										// as coordenadas visíveis na janela
		break;

//==============================================================================
// Terminar e Processamentos default
//==============================================================================
	case WM_CLOSE:
		resposta=MessageBox(hWnd,"Terminar o Programa?", "Fim", MB_YESNO|MB_ICONQUESTION);
		if (resposta==IDYES)
			PostQuitMessage(0);				// Se YES, terminar programa
		break;

// Restantes mensagens têm processamento default
	default:
		return(DefWindowProc(hWnd,messg,wParam,lParam));
		break;
	}
	return(0);
}
