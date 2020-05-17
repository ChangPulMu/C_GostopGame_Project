#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int total_ucards=48;
int game_over=0;
int nagari=0;
int ucard_save=0;
int gcard_save=0;
int a_hcard_save=0;
int b_hcard_save=0;
int c_hcard_save=0;
int a_dcard_save=0;
int b_dcard_save=0;
int c_dcard_save=0;	

typedef struct undrawed_card { // 화투더미
	int card;
	int card_num;
	struct undrawed_card * next;
}Ucard;

typedef struct have_card { // 먹은 패
	int card;
	int gu_ten;
	int hongdan;
	int chungdan;
	int chodan;
	int bigwang;
	int godori;
	int pi_num_counted;
	int ten_num_counted;
	int five_num_counted;
	int kwang_num_counted;
	int hongdan_counted;
	int chungdan_counted;
	int chodan_counted;
	int godori_counted;
	struct have_card * next;
}Hcard;

typedef struct game_card { // 깔린 패
	int card;
	int first_card;
	int total_cards;
	struct game_card * next;
}Gcard;

typedef struct drawed_card { // 든 패 
	int card;
	int card_num;
	struct drawed_card * next;
}Dcard;

typedef struct player {
	Hcard * hcard;
	Dcard * dcard;
	int order;
	int winner;
	int ChongTong;
	int heundel;
	int go;
	int mungtungguri;
	int score;
	int money;
	int bbuk;
	int gobak;
}Player;

Player a = {0}, b = {0}, c={0};

void MixCard(int card[]); // 카드 섞어주는 함수
void Ucard_Initialize(int card[], int num, Ucard* ucard); // 화투 더수미 랜덤하게 초기화
void UcardMinus(Ucard* ucard); // 화투 더미에서 카드 하나 뽑았을 때 카드 재정렬 함수
void Gcard_Initialize(Ucard* ucard, Gcard* gcard); // 화투더미에서 든 패로 카드 옮기는 함수
void Dcard_Initialize(Ucard* ucard, Dcard* dcard); // 화투더미에서 깔린 패로 카드 옮기는 함수
void PlaySetting(Ucard* ucard, Gcard* gcard, Player * a, Player *b, Player *c); // 화투더미에서 깔린 패로 7장, 각 플레이어에게 6장씩 나눠주는 함수
void PrintFunction(int card); // 카드의 정보 출력하는 함수
void gcard_State(Gcard* gcard); // 깔린 패의 정보 출력 함수
void hcard_State(Hcard* hcard); // 먹은 패의 정보 출력 함수
void dcard_State(Dcard* dcard); // 든 패의 정보 출력 함수
void PrintCurrentState(Gcard* gcard, Player * a, Player *b, Player *c); // 각 플레이어가 든 패와 먹은 패를 출력하는 함수
void GcardAdd(Gcard* gcard, int num); // 깔린 패가 사라지면 깔린 패를 추가하는 함수
void GcardMinus(Gcard* gcard); // 먹은 깔린 패를 없애는 함수
void HcardAdd(Hcard* hcard, int num); // 먹은 카드를 먹은 패에 추가하는 함수
void HcardMinus(Hcard* hcard); // 먹은 패를 회수하는 함수
void DcardMinus(Dcard* dcard); // 든 패를 없애는 함수 
void DcardArrange(Dcard* dcard); // 깔린 패를 순서에 맞춰 정렬하는 함수
void GcardArrange(Gcard* gcard); // 깔린 패를 순서에 맞춰 정렬하는 함수
void HcardArrange(Hcard* hcard); // 먹은 패를 순서에 맞춰 정렬하는 함수
int CardCompare(Ucard* ucard, Gcard* gcard, Gcard* initial_gcard, Player* player, Dcard* dcard, int dcard_num); // 게임 진행에서 대부분의 규칙을 검사하는 함수
int OrderSelection(Player *player, Ucard* ucard, Gcard* gcard, char * order); // 시스템 종료, 다시명령, 게임 종료 및 게임 재시작, 다음 시퀀스로 이동, 로드 를 검사하는 함수
void CardVerify(Player *player);
void ScoreCalculation(Player *player, int pi_num, int ten_num, int five_num, int gwang_num);
void HelpPrint(); // 도움말을 출력해주는 함수
int ChongTongCheck(Player* a, Player* b, Player* c); // 총통을 체크하는 함수
void EndSequence(Player *a, Player *b, Player *c); // 어떤 플레이어의 승리인지 검사한 후 게임을 마무리하는 함수
void MoneyCalculate(Player* winner, Player *loser1, Player *loser2); // 점수를 계산하는 함수
void WinnerFinalScore(Player *player); // 이긴 플레이어의 플러스 점수까지 계싼해 주는 함수
void Gu_Ten_Exchange(Hcard *hcard); // 9월 쌍피를 9월 십으로 계산할 수 있게 바꾸어 주는 함수
void AllReset(Ucard* ucard, Gcard* gcard, Player* a, Player* b, Player* c); // 깔린 패와 화투더미와 각각의 플레이어의 구조체 초기화 하는 함수
void Load_Sequence(Player* a, Player* b, Player *c, Ucard* ucard, Gcard* gcard); // save.txt파일에 저장된 정보를 읽어와 현재의 고스톱 코드의 각각의 변수에 그 정보를 저장시켜주는 함수
void Save_Sequence(Player* a, Player* b, Player *c, Ucard* ucard, Gcard* gcard); // 현 상황을 save.txt파일에 기록해주는 함수
void my_bbuk(Player* player1, Player* player2, Player* player3); // 뻑으로 인한 카드를 주는 함수
void Heundel(Player* player); // 흔들기를 판단하고 알리는 함수


int main(void)
{
	int everycard[48] = { 10,12,13,14, \
						  21,22,23,24, \
						  30,32,33,34, \
						  41,42,43,44, \
						  51,52,53,54, \
						  61,62,63,64, \
						  71,72,73,74, \
						  80,81,83,84, \
						  92,93,94,95, \
						  101,102,103,104, \
						  110,113,114,115, \
						  120,121,122,125};
	char order[10], exit_agree[10];
	int CHcheck, exit_check;
	Ucard* ucard = (Ucard *)calloc(1, sizeof(Ucard));
	Gcard* gcard = (Gcard *)calloc(1, sizeof(Gcard));
	a.money = 100000, b.money = 100000, c.money = 100000;
	a.order=1;


Game_Start:
	ucard = (Ucard *)calloc(1, sizeof(Ucard));
	gcard = (Gcard *)calloc(1, sizeof(Gcard));
	a.dcard = (Dcard *)calloc(1, sizeof(Dcard));
	a.hcard = (Hcard *)calloc(1, sizeof(Hcard));
	b.dcard = (Dcard *)calloc(1, sizeof(Dcard));
	b.hcard = (Hcard *)calloc(1, sizeof(Hcard));
	c.dcard = (Dcard *)calloc(1, sizeof(Dcard));
	c.hcard = (Hcard *)calloc(1, sizeof(Hcard));

	srand((unsigned)time(NULL));
	MixCard(everycard);
	Ucard_Initialize(everycard, 47, ucard);
	PlaySetting(ucard, gcard, &a, &b, &c);
	CHcheck = ChongTongCheck(&a, &b, &c); // -1반환시 총통이 일어남
	printf("%d %d %d", a.score, b.score, c.score);
	if(CHcheck == -1)
	{
		if(a.winner==1)
		{
			MoneyCalculate(&a, &b, &c);
			a.order=1;
		}
		else if(b.winner==1)
		{
			MoneyCalculate(&b, &a, &c);
			b.order=1;
		}
		else
		{
			MoneyCalculate(&c, &a, &b);
			c.order=1;
		}
		AllReset(ucard, gcard, &a, &b, &c);
		CHcheck = 0;
		printf("3초 후 게임을 새로 시작합니다.\n");
		goto Game_Start;
	}
Loadsequence:

	if(a.order == 1)
		goto Sequence_A;
	else if(b.order==1)
		goto Sequence_B;
	else if(c.order==1)
		goto Sequence_C;

	while(1)
	{

Sequence_A:

		PrintCurrentState(gcard, &a, &b, &c);
		Heundel(&a);
		Heundel(&b);
		Heundel(&c);
		printf("A의 차례\n");
		printf("명령: ");
		scanf("%s", order);
		exit_check = OrderSelection(&a, ucard, gcard, order);
		if(exit_check==1)
		{
			EndSequence(&a, &b, &c);
			AllReset(ucard, gcard, &a, &b, &c);
			CHcheck = 0;
			exit_check=0;
			a.winner=0;
			printf("플레이어 A의 승리입니다!\n");
			printf("3초 후에 게임을 새로 시작합니다.\n");
			goto Game_Start;
		}
		else if(exit_check==3)
		{	
			a.order=1;
			b.order=0;
			c.order=0;
			goto Loadsequence;
		}
		else if(exit_check == -1)
		{
			while(exit_check == -1)
			{

				printf("정말로 게임을 종료합니까? (y/n) : ");
				scanf("%s", exit_agree);
				if(exit_agree[0] == 'y')
				{
					exit_check=0;
					printf("2초 후 게임을 종료합니다.\n");
					return 0;
				}
				else if(exit_agree[0] == 'n')
				{
					exit_check=0;
					printf("게임을 진행합니다.\n");
					goto Sequence_A;
				}	
				else
				{
					exit_check=0;
					printf("잘못된 명령입니다. 게임을 진행합니다.\n");
					goto Sequence_A;
				}
			}
		}
		else if(exit_check == 0)
		{
			goto Sequence_A;
		}
		else if(exit_check == 4)
		{
			AllReset(ucard, gcard, &a, &b, &c);
			CHcheck=0;
			exit_check=0;
			a.winner=0;
			printf("나가리로 게임이 끝났습니다.\n");
			printf("3초 후에 게임을 새로 시작합니다.\n");
			goto Game_Start;
		}
		a.order=0;
		b.order=1;
		c.order=0;
Sequence_B:
		PrintCurrentState(gcard, &a, &b, &c);
		Heundel(&a);
		Heundel(&b);
		Heundel(&c);
		printf("B의 차례\n");
		printf("명령: ");
		scanf("%s", order);
		exit_check = OrderSelection(&b, ucard, gcard, order);
		if(exit_check==1)
		{
			EndSequence(&a, &b, &c);
			AllReset(ucard, gcard, &a, &b, &c);
			CHcheck = 0;
			exit_check=0;
			b.winner=0;
			printf("플레이어 B의 승리입니다!\n");
			printf("3초 후에 게임을 새로 시작합니다.\n");
			goto Game_Start;
		}
		else if(exit_check==3)
		{
			a.order=0;
			b.order=1;
			c.order=0;
			goto Loadsequence;
		}
		else if(exit_check == -1)
		{
			while(exit_check == -1)
			{
				printf("정말로 게임을 종료합니까? (y/n) : ");
				scanf("%s", exit_agree);
				if(exit_agree[0] == 'y')
				{
					exit_check=0;
					printf("2초 후 게임을 종료합니다.\n");
					return 0;
				}
				else if(exit_agree[0] == 'n')
				{
					exit_check=0;
					printf("게임을 진행합니다.\n");
					goto Sequence_B;
				}	
				else
				{
					exit_check=0;
					printf("잘못된 명령입니다. 게임을 진행합니다.\n");
					goto Sequence_B;
				}
			}
		}
		else if(exit_check == 0)
		{
			goto Sequence_B;
		}
		else if(exit_check == 4)
		{
			AllReset(ucard, gcard, &a, &b, &c);
			CHcheck=0;
			exit_check=0;
			b.winner=0;
			printf("나가리로 게임이 끝났습니다.\n");
			printf("3초 후에 게임을 새로 시작합니다.\n");
			goto Game_Start;
		}
		a.order=0;
		b.order=0;
		c.order=1;
Sequence_C:
		
		PrintCurrentState(gcard, &a, &b, &c);
		Heundel(&a);
		Heundel(&b);
		Heundel(&c);
		printf("C의 차례\n");
		printf("명령: ");
		scanf("%s", order);
		exit_check = OrderSelection(&c, ucard, gcard, order);
		if(exit_check==1)
		{
			EndSequence(&a, &b, &c);
			AllReset(ucard, gcard, &a, &b, &c);
			CHcheck = 0;
			exit_check=0;
			c.winner=0;
			printf("플레이어 C의 승리입니다!\n");
			printf("3초 후에 게임을 새로 시작합니다.\n");
			goto Game_Start;
		}
		else if(exit_check==3)
		{
			a.order=0;
			c.order=1;
			b.order=0;
			goto Loadsequence;
		}
		else if(exit_check == -1)
		{
			while(exit_check == -1)
			{
				printf("정말로 게임을 종료합니까? (y/n) : ");
				scanf("%s", exit_agree);
				if(exit_agree[0] == 'y')
				{
					exit_check=0;
					printf("2초 후 게임을 종료합니다.\n");
					return 0;
				}
				else if(exit_agree[0] == 'n')
				{
					exit_check=0;
					printf("게임을 진행합니다.\n");
					goto Sequence_C;
				}	
				else
				{
					exit_check=0;
					printf("잘못된 명령입니다. 게임을 진행합니다.\n");
					goto Sequence_C;
				}
			}
		}	
		else if(exit_check == 0)
		{
			goto Sequence_C;
		}
		else if(exit_check == 4)
		{
			AllReset(ucard, gcard, &a, &b, &c);
			CHcheck=0;
			exit_check=0;
			c.winner=0;
			printf("나가리로 게임이 끝났습니다.\n");
			printf("3초 후에 게임을 새로 시작합니다.\n");
			goto Game_Start;
		}
		a.order=1;
		b.order=0;
		c.order=0;
	}
	return 0;
}


void MixCard(int card[])
{
	int i = 0, random, temp;
	while (i <= 47)
	{
		random = rand() % 48;
		temp = card[i];
		card[i] = card[random];
		card[random] = temp;
		i++;
	}
}

void GcardAdd(Gcard* gcard, int num)
{
	int temp;
	if(gcard->next == NULL)
	{
		temp = gcard->card;
		gcard->next = (Gcard*)calloc(1, sizeof(Gcard));
		gcard->card = num;
		gcard->next->card = temp;
		return;
	}
	else
	{
		temp = gcard->card;
		gcard->card = num;
		GcardAdd(gcard->next, temp);


	}
}

void GcardMinus(Gcard* gcard)
{
	if(gcard->next==NULL)
	{
		if(gcard->card !=0)
		{
			gcard->card=0;
			return;
		}
		else
		{
			printf("깔린 패가 더 이상 존재하지 않습니다.\n");
			return;
		}
	}
	else if(gcard->next->next == NULL)
	{
		gcard->card = gcard->next->card;
		free(gcard->next);
		gcard->next=NULL;
		return;
	}
	else
	{
		gcard->card = gcard->next->card;
		GcardMinus(gcard->next);
	}
}

void HcardAdd(Hcard* hcard, int num)
{
	int temp;
	if(num==95)
	{
		hcard->gu_ten =1;
	}
	if(hcard->next == NULL)
	{
		temp = hcard->card;
		hcard->next = (Hcard*)calloc(1, sizeof(Hcard));
		hcard->card = num;
		hcard->next->card = temp;
		return;
	}
	else
	{
		temp = hcard->card;
		hcard->card = num;
		HcardAdd(hcard->next, temp);
	}
}

void HcardMinus(Hcard* hcard)
{
	if(hcard->next==NULL)
	{
		if(hcard->card !=0)
		{
			hcard->card=0;
			return;
		}
		else
		{
			printf("딴 패가 더 이상 존재하지 않습니다.\n");
			return;
		}
	}
	else if(hcard->next->next == NULL)
	{
		hcard->card = hcard->next->card;
		free(hcard->next);
		hcard->next=NULL;
		return;
	}
	else
	{
		hcard->card = hcard->next->card;
		HcardMinus(hcard->next);
	}
}

void DcardMinus(Dcard* dcard)
{
	if(dcard->next==NULL)
	{
		if(dcard->card !=0)
		{
			dcard->card=0;
			return;
		}
		else
		{
			printf("가지고 있는 패가 더 이상 존재하지 않습니다.\n");
			return;
		}
	}
	else if(dcard->next->next == NULL)
	{
		dcard->card = dcard->next->card;
		free(dcard->next);
		dcard->next=NULL;
		return;
	}
	else
	{
		dcard->card = dcard->next->card;
		DcardMinus(dcard->next);
	}
}

void DcardAdd(Dcard* dcard, int num)
{
	int temp;
	if(dcard->next == NULL)
	{
		temp = dcard->card;
		dcard->next = (Dcard*)calloc(1, sizeof(Dcard));
		dcard->card = num;
		dcard->next->card = temp;
		return;
	}
	else
	{
		temp = dcard->card;
		dcard->card = num;
		DcardAdd(dcard->next, temp);


	}
}

void HcardArrange(Hcard* hcard)
{
	static int h_save = 0;
	Hcard* hcardsave = hcard;
	int temp=0, temp_save=0;

	while(hcard->next != NULL)
	{
		hcard = hcard->next;
		temp_save++;
	}

	if((temp_save - h_save) <= 1)
	{
		h_save=0;
		return;
	}

	hcard = hcardsave;
	
	while((temp_save-h_save) > 1)
	{
		if((hcard->card) > (hcard->next->card))
		{
			temp = hcard->card;
			hcard->card = hcard->next->card;
			hcard->next->card = temp;
		}
		hcard = hcard->next;
		temp_save--;
	}
	h_save++;
	hcard = hcardsave;
	HcardArrange(hcard);
}

void DcardArrange(Dcard* dcard)
{
	static int d_save = 0;
	Dcard* dcardsave = dcard;
	int temp=0, temp_save=0;

	while(dcard->next != NULL)
	{
		dcard = dcard->next;
		temp_save++;
	}

	if((temp_save - d_save) <= 1)
	{
		d_save=0;
		return;
	}

	dcard = dcardsave;
	
	while((temp_save-d_save) > 1)
	{
		if((dcard->card) > (dcard->next->card))
		{
			temp = dcard->card;
			dcard->card = dcard->next->card;
			dcard->next->card = temp;
		}
		dcard = dcard->next;
		temp_save--;
	}
	d_save++;
	dcard = dcardsave;
	DcardArrange(dcard);
}

void GcardArrange(Gcard* gcard)
{
	static int g_save = 0;
	Gcard* gcardsave = gcard;
	int temp=0, temp_save=0;

	while(gcard->next != NULL)
	{
		gcard = gcard->next;
		temp_save++;
	}

	if((temp_save - g_save) <= 1)
	{
		g_save=0;
		return;
	}

	gcard = gcardsave;
	
	while((temp_save-g_save) > 1)
	{
		if((gcard->card) > (gcard->next->card))
		{
			temp = gcard->card;
			gcard->card = gcard->next->card;
			gcard->next->card = temp;
		}
		gcard = gcard->next;
		temp_save--;
	}
	g_save++;
	gcard = gcardsave;
	GcardArrange(gcard);
}

void Ucard_Initialize(int card[], int num, Ucard* ucard) // 화투 더미 랜덤하게 초기화
{
	ucard->next = NULL;
	if (num < 0)
	{
		return;
	}
	else
	{
		ucard->card = card[47 - num];
		num--;
		ucard->next = (Ucard *)calloc(1,sizeof(Ucard));
		Ucard_Initialize(card, num, ucard->next);
	}
}

void UcardMinus(Ucard* ucard) // 화투 더미에서 카드 하나 뽑았을 때 카드 재정렬 함수
{
	total_ucards--;
	if(ucard->next==NULL)
	{
		if(ucard->card !=0)
		{
			ucard->card=0;
			return;
		}
		else
		{
			printf("화투더미에 패가 더 이상 존재하지 않습니다.\n");
			return;
		}
	}
	else if (ucard->next->next == NULL)
	{
		ucard->card  = ucard->next->card;
		free(ucard->next);
		ucard->next = NULL;
		return;
	}
	else
	{
		ucard->card = ucard->next->card;
		UcardMinus(ucard->next);
	}
}

void PlaySetting(Ucard* ucard, Gcard* gcard, Player * a, Player *b, Player *c) // 화투더미에서 깔린 패로 6장, 각 플레이어에게 7장씩 나눠주는 함수
{
	int i;
	for (i = 6; i > 0; i--)
	{
		GcardAdd(gcard,ucard->card);
		UcardMinus(ucard);
		GcardArrange(gcard);
	}

	for (i =7 ; i > 0; i--)
	{
		DcardAdd(a->dcard, ucard->card);
		UcardMinus(ucard);
		DcardArrange(a->dcard);
	};

	for (i = 7; i > 0; i--)
	{
		DcardAdd(b->dcard, ucard->card);
		UcardMinus(ucard);
		DcardArrange(b->dcard);
	};

	for (i = 7; i > 0; i--)
	{
		DcardAdd(c->dcard, ucard->card);
		UcardMinus(ucard);
		DcardArrange(c->dcard);
	};

}

void PrintFunction(int card) // 카드의 정보 출력하는 함수
{
	if (card%10 == 0)
		printf("%d광 ", card / 10);
	else if (card%10 == 1)
		printf("%d십 ", card / 10);
	else if (card%10 == 2)
		printf("%d오 ", card / 10);
	else if (card%10 == 3 || card%10 == 4)
		printf("%d피%d ", card / 10, (card%10)-2);
	else if (card%10 == 5)
		printf("%d쌍피 ", card / 10);
}

void gcard_State(Gcard* gcard) // 깔린 패의 정보 출력 함수
{
	if (gcard->card != 0)
	{
		PrintFunction(gcard->card);
		gcard_State(gcard->next);
	}
	else
	{
		printf("\n");
		return;
	}
}

void hcard_State(Hcard* hcard) // 먹은 패의 정보 출력 함수
{
	if (hcard->card != 0)
	{
		PrintFunction(hcard->card);
		hcard_State(hcard->next);
	}
	else
	{
		printf("\n");
		return;
	}
}

void dcard_State(Dcard* dcard) // 든 패의 정보 출력 함수
{
	if (dcard->card != 0)
	{
		PrintFunction(dcard->card);
		dcard_State(dcard->next);
	}
	else
	{
		printf("\n");
		return;
	}
}

void PrintCurrentState(Gcard* gcard, Player * a, Player *b, Player *c) // 각 플레이어가 든 패와 먹은 패를 출력하는 함수
{
	GcardArrange(gcard);
	DcardArrange(a->dcard);
	DcardArrange(b->dcard);
	DcardArrange(c->dcard);
	HcardArrange(a->hcard);
	HcardArrange(b->hcard);
	HcardArrange(c->hcard);

	printf("A ");
	if(a->heundel == 1)
		printf("흔듦 : ");
	else
		printf("	 : ");
	dcard_State(a->dcard);
	CardVerify(a);
	printf("(점수: %d) 딴 화투) ", a->score);
	hcard_State(a->hcard);

	printf("B ");
	if(b->heundel == 1)
		printf("흔듦 : ");
	else
		printf("	 : ");
	dcard_State(b->dcard);
	CardVerify(b);
	printf("(점수: %d) 딴 화투) ", b->score);
	hcard_State(b->hcard);

	printf("C ");
	if(c->heundel == 1)
		printf("흔듦 : ");
	else
		printf("	 : ");
	dcard_State(c->dcard);
	CardVerify(c);
	printf("(점수: %d) 딴 화투) ", c->score);
	hcard_State(c->hcard);

	printf("\n깔린 패) ");
	gcard_State(gcard);

}


int CardCompare(Ucard* ucard, Gcard* gcard, Gcard* initial_gcard, Player* player, Dcard* dcard, int dcard_num)
{
	static int stop = 0;
	static int second_sequence=0;
	static int go_sequence=0;
	static int choose_one = 0;
	static int card_save[4];
	static int card_sequence=0;
	static int bbuk_card=0;
	static int score_save;
	char go_order[10];
	int count;
	int double_pi=0;
	int pi_moved=0;
	int choose_one_order;
	int my_buk_sequence=0;
	int i;
	int gcard_num=0;

	go_sequence=0;

	if(second_sequence==0)
		score_save=player->score;
	else
		;
	
	static int card_save1=0, card_save2=0;
	Hcard* player_hcard_save1=NULL;
	Hcard* player_hcard_save2=NULL;
	Gcard* gcard_save1=NULL;
	Gcard* gcard_save2=NULL;
	Gcard* gcard_save_for_go=initial_gcard;

	while(gcard_save_for_go->card != 0)
	{
		gcard_num++;
		gcard_save_for_go = gcard_save_for_go->next;
	}

	if(stop==0)
	{
		for(count=1; count<dcard_num; count++)
		{
			dcard = dcard->next;
			if(dcard == NULL || dcard->card == 0)
			{
				printf("잘못 입력하셨습니다.\n");
				return 3;
			}
		}
		stop=1;
		CardCompare(ucard, gcard, initial_gcard, player, dcard, dcard_num);
	}

	else
	{
		while(gcard->card!=0)
		{
			if((gcard->card)/10 == (dcard->card)/10) // 낸 카드가 깔린 카드와 맞았을 경우
			{

				printf("%d와 %d가 맞았습니다!\n", gcard->card, dcard->card);
				CardVerify(player);
				gcard_save1 = gcard;
				gcard = gcard->next;
			
				while(gcard->card!=0)
				{
					if((gcard->card)/10 == (dcard->card)/10)
					{
						gcard_save2 = gcard;
					}
					gcard = gcard->next;
				}

				if(bbuk_card != 0 && ((gcard_save1->card)/10 == bbuk_card)) // 뻑을 먹었을 때
				{
					printf("뻑을 먹었습니다!\n");
					bbuk_card=0;
					HcardAdd(player->hcard, dcard->card);
					DcardMinus(dcard);
					HcardAdd(player->hcard, gcard_save1->card);
					GcardMinus(gcard_save1);
					HcardAdd(player->hcard, gcard_save1->card);
					GcardMinus(gcard_save1);
					HcardAdd(player->hcard, gcard_save1->next->next->card);
					GcardMinus(gcard_save1);
					if(player->bbuk == 1) // 자기 뻑일때
					{
						player->bbuk=0;

						if(player==&a)
							for(i=0; i<2; i++)
								my_bbuk(player, &b, &c);
						else if (player==&b)
							for(i=0; i<2; i++)
								my_bbuk(player, &a, &c);
							else
								for(i=0; i<2; i++)
									my_bbuk(player, &a, &b);
					}

					else // 자기 뻑이 아닐 때
					{
						a.bbuk=0, b.bbuk=0, c.bbuk=0;
						if(player==&a)
							my_bbuk(player, &b, &c);
						else if (player==&b)
							my_bbuk(player, &a, &c);
						else 
							my_bbuk(player, &a, &b);
					}	
				}

				if ( card_sequence == 0 && second_sequence == 1 && (card_save[0]/10) == (card_save[1]/10)) // 쪽
				{
					printf("쪽입니다!\n");
					if(player == &a)
						my_bbuk(player, &b, &c);
					else if (player==&b)
						my_bbuk(player, &a, &c);
					else
						my_bbuk(player, &a, &b);
				}

				if(gcard_save2!=NULL)
					choose_one=1;
			
				if(choose_one==1)
				{
					while(choose_one==1)
					{
						printf("\n");
						PrintFunction(gcard_save1->card); printf("와 "); PrintFunction(gcard_save2->card); printf(" 중 어떤 것을 먹으시겠습니까? (1/2) :");
						scanf("%d", &choose_one_order);
						if(choose_one_order==1)
						{
							gcard = gcard_save1;
							gcard_save2=NULL;
							choose_one=0;
						}
						else if(choose_one_order==2)
						{
							gcard = gcard_save2;
							gcard_save2=NULL;
							choose_one=0;
						}
						else
							printf("잘못 입력하셨습니다. 다시 입력해주세요.");
					}
				}
				else
					gcard = gcard_save1;
			
				card_save[card_sequence++] = dcard->card; // 뻑/따닥/쪽 체크 
				if( card_sequence == 3 && (card_save[0]/10 == card_save[1]/10) && (card_save[1]/10 == card_save[2]/10) && card_save[0]!=0 && card_save[1] !=0 && card_save[2] != 0) // 뻑
				{
					printf("뻑입니다!\n");
					bbuk_card = card_save[0]/10;
					GcardAdd(gcard, dcard->card);
					GcardAdd(gcard, player->hcard->card);
					HcardMinus(player->hcard);
					GcardAdd(gcard, player->hcard->card);
					HcardMinus(player->hcard);
					player->bbuk=1;
					printf("플레이어 뻑: %d", player->bbuk);
					stop=0;
					second_sequence=0;
					go_sequence=0;
					choose_one=0;
					for(i=0; i<4; i++)
						card_save[i] = 0;
					card_sequence=0;
					score_save=0;
					bbuk_card=0;
					return 0;
				}
				card_save[card_sequence++] = gcard_save1->card;
				if( card_sequence == 4 && (card_save[0]/10 == card_save[1]/10) && (card_save[2]/10 == card_save[3]/10) && (card_save[0]/10 == card_save[2]/10&& card_save[0]!=0 && card_save[1] !=0 && card_save[2] != 0)) // 따닥
				{
					printf("따닥입니다!\n");
						if(player == &a)
							my_bbuk(player, &b, &c);
						else if (player == &b)
							my_bbuk(player, &a, &c);
						else
							my_bbuk(player ,&a, &b);
				}
	
				HcardAdd(player->hcard, gcard->card); // 먹은 패에 카드 2개 추가
				HcardAdd(player->hcard, dcard->card);
				GcardMinus(gcard);
				DcardMinus(dcard);		
				CardVerify(player);

				if(dcard->card == 0)
				{
					if(initial_gcard->card==0 && ucard->card != 0) // 싹쓸이 시퀀스
					{
						printf("싹쓸이 하셨습니다.\n");
						if(player==&a)
							my_bbuk(player, &b, &c);
						else if(player==&b)
							my_bbuk(player, &a, &c);
						else
							my_bbuk(player, &a, &b);
					}

					else if (score_save<player->score && player->score >= 3)
					{
						player->winner =1;
						stop=0;
						second_sequence=0;
						go_sequence=0;
						choose_one=0;
						for(i=0; i<4; i++)
							card_save[i] = 0;
						card_sequence=0;
						score_save=0;
						bbuk_card=0;
						return 1;
					}
					else if(initial_gcard->card == 0 && ucard->card ==0 && score_save >= player->score) 
					{
						nagari=1;
						printf("나가리로 게임이 모두 끝났습니다.\n");
						stop=0;
						second_sequence=0;
						go_sequence=0;
						choose_one=0;
						for(i=0; i<4; i++)
							card_save[i] = 0;
						card_sequence=0;
						score_save=0;
						bbuk_card=0;
						return 4;
					}
				}

				if(second_sequence==1) // 두 번째 시퀀스라면
				{
					PrintCurrentState(initial_gcard, &a, &b, &c); 
					printf("두번째 시퀀스에 맞았습니다!\n");
					second_sequence=0;
					stop=0;
					if(player->score >= 3)
					{
						if((a.go != 0 || b.go != 0 || c.go !=0) && player->go == 0)
							go_sequence=2;

						else if(score_save < player->score)
							go_sequence=1;
						else
							go_sequence=0;
					}
					score_save=0;

					if(go_sequence==2)
					{
						printf("고박입니다.\n");
						player->winner=1;
						if(a.go == 1)
							a.gobak=1;
						else if(b.go==1)
							b.gobak=1;
						else
							c.gobak=1;
						stop=0;
						second_sequence=0;
						go_sequence=0;
						choose_one=0;
						for(i=0; i<4; i++)
							card_save[i] = 0;
						card_sequence=0;
						score_save=0;
						bbuk_card=0;
						return 1;
					}

					else if(go_sequence==1) // go시퀀스
					{
						if(gcard_num<=3)
							;
						else
						{
							while(go_sequence==1)
							{
								printf("\n고 하시겠습니까? (g/s): ");
								scanf("%s", go_order);
								if(!strcmp(go_order, "g"))
								{
									player->go++;
									stop=0;
									second_sequence=0;
									go_sequence=0;
									choose_one=0;
									for(i=0; i<4; i++)
										card_save[i] = 0;
									card_sequence=0;
									score_save=0;
									return 0;
								}
								else if (!strcmp(go_order, "s"))
								{
									printf("정말로 스탑하시겠습니까? (y/n) :");
									scanf("%s", go_order);
									if(!strcmp(go_order, "y"))
									{
										player->winner =1;
										stop=0;
										second_sequence=0;
										go_sequence=0;
										choose_one=0;
										for(i=0; i<4; i++)
											card_save[i] = 0;
										card_sequence=0;
										score_save=0;
										bbuk_card=0;
										return 1;
									}
									else if (!strcmp(go_order, "n"))
									{
										go_sequence = 1;
									}
									else
									{
										printf("잘못된 명령입니다.\n");
										go_sequence=1;
									}
								}
								else
								{
									printf("잘못된 명령입니다. 다시 입력해주세요.\n");
									go_sequence=1;
								}
							}
						}
					}
					break;
				}

				else // 첫번째 시퀀스라면
				{
					printf("첫번째 시퀀스에서 맞았습니다!\n");
					DcardAdd(player->dcard, ucard->card); // 카드 하나 뽑기
					PrintFunction(player->dcard->card); printf("를 뽑으셨습니다.\n");
					UcardMinus(ucard);
					card_save1 = dcard->card;
					second_sequence=1;
					stop = 0;
					CardCompare(ucard, initial_gcard, initial_gcard, player, player->dcard, 1);
					break;
				}
			}
			gcard = gcard->next;
		}

		if(gcard_save1 == NULL) // 하나도 맞는 것이 없을 때
		{
			GcardAdd(initial_gcard, dcard->card);
			card_save[0] = dcard->card;
			DcardMinus(dcard);
			
			if(initial_gcard->card == 0)
			{
				if(ucard->card==0)
				{
					nagari=1;
					printf("나가리로 게임이 모두 끝났습니다.\n");
					stop=0;
					second_sequence=0;
					go_sequence=0;
					choose_one=0;
					for(i=0; i<4; i++)
						card_save[i] = 0;
					card_sequence=0;
					score_save=0;
					bbuk_card=0;
					return 4;
				}
				else if (score_save<player->score && player->score >= 3)
				{
					player->winner =1;
					stop=0;
					second_sequence=0;
					go_sequence=0;
					choose_one=0;
					for(i=0; i<4; i++)
						card_save[i] = 0;
					card_sequence=0;
					score_save=0;
					bbuk_card=0;
					return 1;
				}
			}
			if(second_sequence==1) // 두 번째 시퀀스일 경우
			{
				printf("두번째 시퀀스에 맞는 것이 없었습니다.\n");
				second_sequence=0;
				stop=0;
				if(player->score >= 3)
				{
					if((a.go != 0 || b.go != 0 || c.go !=0) && player->go == 0)
						go_sequence=2;

					else if(score_save < player->score)
						go_sequence=1;
					else
						go_sequence=0;
				}
				score_save=0;

				if(go_sequence==2)
				{
					printf("고박입니다.\n");
					player->winner=1;
					if(a.go == 1)
						a.gobak=1;
					else if(b.go==1)
						b.gobak=1;
					else
						c.gobak=1;
					
					stop=0;
					second_sequence=0;
					go_sequence=0;
					choose_one=0;
					for(i=0; i<4; i++)
						card_save[i] = 0;
					card_sequence=0;
					score_save=0;
					bbuk_card=0;
					return 1;
				}

				else if(go_sequence==1) // go시퀀스
				{
					while(go_sequence==1)
					{
						printf("\n고 하시겠습니까? (g/s): ");
						scanf("%s", go_order);
						if(!strcmp(go_order, "g"))
						{
							player->go++;
							go_sequence=0;
							second_sequence=0;
							stop=0;
							second_sequence=0;
							go_sequence=0;
							choose_one=0;
							for(i=0; i<4; i++)
								card_save[i] = 0;
							card_sequence=0;
							score_save=0;
							return 0;
						}
						else if (!strcmp(go_order, "s"))
						{
							printf("정말로 스탑하시겠습니까? (y/n) :");
							scanf("%s", go_order);
							if(!strcmp(go_order, "y"))
							{
								player->winner =1;

								stop=0;
								second_sequence=0;
								go_sequence=0;
								choose_one=0;
								for(i=0; i<4; i++)
									card_save[i] = 0;
								card_sequence=0;
								score_save=0;
								bbuk_card=0;
								return 1;
							}
							else if (!strcmp(go_order, "n"))
							{
								go_sequence = 1;
							}
							else
							{
								printf("잘못된 명령입니다.\n");
								go_sequence=1;
							}
						}
						else
						{
							printf("잘못된 명령입니다. 다시 입력해주세요.\n");
							go_sequence=1;
						}
					}
				}

				stop=0;
				second_sequence=0;
				go_sequence=0;
				choose_one=0;
				for(i=0; i<4; i++)
					card_save[i] = 0;
				card_sequence=0;
				score_save=0;
				bbuk_card=0;
			}

			else // 첫 번째 시퀀스일 경우
			{
				printf("첫번째 시퀀스에 맞는 것이 없었습니다.\n");
				DcardAdd(player->dcard, ucard->card); // 카드 하나 뽑기
				PrintFunction(player->dcard->card); printf("를 뽑았습니다.\n\n");
				card_save[1]=player->dcard->card;
				UcardMinus(ucard);	
				second_sequence=1;
				stop = 0;
				CardCompare(ucard, initial_gcard, initial_gcard, player, player->dcard, 1);
			}
		}
	}
}

int OrderSelection(Player *player, Ucard* ucard, Gcard* gcard, char * order) // -1반환은 시스템 종료, 0 반환은 다시 명령받기,1반환은 게임 종료 및 게임 재시작, 2반환은 다음 시퀀스로, 3반환은 로드, 4반환은 나가리로 게임 종료
{
	int card_num=0, restart_sequence=0;
	if (order[0] >= '1' && order[0] <= '7')
	{
		card_num = atoi(order);
		restart_sequence = CardCompare(ucard, gcard, gcard, player, player->dcard, card_num);
		if(restart_sequence == 1)
			return 1;
		else if (restart_sequence == 3)
			return 0;
		else if (restart_sequence == 4)
			return 4;
		else
			return 2;
	}

	else if (!strcmp("save", order))
	{
		Save_Sequence(&a, &b, &c, ucard, gcard);
		return 0;
	}

	else if (!strcmp("load", order))
	{
		Load_Sequence(&a, &b, &c, ucard, gcard);
		return 3;
	}

	else if (order[0] == 'b' || !strcmp("balance", order))
	{
		printf("플레이어A의 잔고는 %d원 입니다.\n", a.money);
		printf("플레이어B의 잔고는 %d원 입니다.\n", b.money);
		printf("플레이어C의 잔고는 %d원 입니다.\n", c.money);
		return 0;
	}
	else if (order[0] == 'h' || !strcmp("help", order))
	{
		HelpPrint();
		return 0;
	}
	else if (order[0] == '9')
	{
		if( player->hcard->gu_ten == 1)
		{
			Gu_Ten_Exchange(player->hcard);
			printf("9십을 피에서 십으로 옮겼습니다.");
		}
		else
		{
			printf("잘못된 명령입니다. 게임을 진행합니다.\n");
		}
		return 0;
	}
	else if (order[0] == 'e' || !strcmp("exit", order))
	{
		return -1;
	}
	else
	{
		printf("잘못된 명령입니다. 다시 입력해 주세요\n");
		return 0;
	}
}

void CardVerify(Player *player) // 카드의 개수, 
{
	int pi_num=0, ten_num=0, five_num=0, kwang_num=0;
	int godori=0, hongdan=0, chungdan=0, chodan=0, bigwang=0;
	int hsave[48]={0};
	int n=0;
	Hcard* hcard = player->hcard;
	while(hcard->card != 0)
	{
		if ((hcard->card)%10 == 0)
		{
			if(hcard->card == 120)
				player->hcard->bigwang = 1;
			kwang_num++;
		}

		else if ((hcard->card)%10 == 1)
		{
			if((hcard->card)/10 == 2 || (hcard->card)/10 == 4 || (hcard->card)/10 == 8)
				godori++;
			ten_num++;
		}

		else if ((hcard->card)%10 == 2)
		{
			if((hcard->card)/10 == 1 || (hcard->card)/10 == 2 || (hcard->card)/10 == 3)
				hongdan++;
			else if((hcard->card)/10 == 4 || (hcard->card)/10 == 5 || (hcard->card)/10 == 7)
				chodan++;
			else if ((hcard->card)/10 == 6 || (hcard->card)/10 == 9 || (hcard->card)/10 == 10)
				chungdan++;
			five_num++;
		}

		else if ((hcard->card)%10 == 3 || (hcard->card)%10 == 4)
		{
			pi_num++;
		}
		else if ((hcard->card)%10 == 5)
		{
			pi_num += 2;
		}
		hcard = hcard->next;
	}

	if(ten_num == 7)
		player->mungtungguri = 1;
	if(hongdan == 3)
		player->hcard->hongdan=1;
	if(chungdan == 3)
		player->hcard->chungdan=1;
	if(chodan == 3)
		player->hcard->chodan=1;
	if(godori ==3)
		player->hcard->godori = 1;

	ScoreCalculation(player, pi_num, ten_num, five_num, kwang_num);
}

void ScoreCalculation(Player* player, int pi_num, int ten_num, int five_num, int kwang_num)
{
	if(pi_num >= 10)
	{
		if(player->hcard->pi_num_counted != 0)
		{
			player->score += pi_num - player->hcard->pi_num_counted;
			player->hcard->pi_num_counted += (pi_num - player->hcard->pi_num_counted);
		}
		else
		{
			printf("피를 10개 이상 모았습니다!\n");
			player->score += pi_num - 9;
			player->hcard->pi_num_counted = pi_num;
		}
	}
	if(ten_num >= 5)
	{
		if(player->hcard->ten_num_counted != 0)
		{
			player->score += ten_num - player->hcard->ten_num_counted;
			player->hcard->ten_num_counted += (ten_num - player->hcard->ten_num_counted);
		}
		else
		{
			printf("십을 5개 이상 모았습니다!\n");
			player->score += ten_num - 4;
			player->hcard->ten_num_counted = ten_num;
		}
	}
	if(five_num >= 5)
	{
		if(player->hcard->five_num_counted != 0)
		{
			player->score += five_num - player->hcard->five_num_counted;
			player->hcard->five_num_counted += (five_num - player->hcard->five_num_counted);
		}
		else
		{
			printf("오를 5개 이상 모았습니다!\n");
			player->score += five_num - 4;
			player->hcard->five_num_counted = five_num;
		}
	}
	if(kwang_num>= 3)
	{
		if(kwang_num == 3 && player->hcard->bigwang == 1)
		{
			if(player->hcard->kwang_num_counted >= 3)
				;
			else
			{
				printf("3광입니다!\n");
				player->hcard->kwang_num_counted = 3;
				player->score += 2;
			}
		}
		else if(kwang_num == 3 && player->hcard->bigwang == 0)
		{
			if(player->hcard->kwang_num_counted >= 3)
				;
			else
			{
				printf("3광입니다!\n");
				player->hcard->kwang_num_counted = 3;
				player->score += 3;
			}
		}
	}
	else if(kwang_num == 4)
	{
		if(player->hcard->kwang_num_counted >= 4)
			;
		else
		{
					printf("4광입니다!\n");
			player->hcard->kwang_num_counted = 4;
			player->score += 4;
		}
	}	
	else if(kwang_num == 5)
	{
		if(player->hcard->kwang_num_counted == 5)
			;
		else
		{
			player->hcard->kwang_num_counted = 5;
			printf("5광입니다!\n");
			player->score += 15;
		}
	}

	if(player->hcard->hongdan == 1)
	{
		if(player->hcard->hongdan_counted == 1)
			;
		else
		{
			printf("홍단입니다!\n");
			player->hcard->hongdan_counted = 1;
			player->score += 3;
		}
	}
	if(player->hcard->chungdan == 1)
	{
		if(player->hcard->chungdan_counted == 1)
			;
		else
		{
			printf("청단입니다!\n");
			player->hcard->chungdan_counted = 1;
			player->score += 3;
		}
	}
	if(player->hcard->chodan == 1)
	{
		if(player->hcard->chodan_counted == 1)
			;
		else
		{
			printf("초단입니다!\n");
			player->hcard->chodan_counted = 1;
			player->score += 3;
		}
	}
	if(player->hcard->godori == 1)
	{
		if(player->hcard->godori_counted == 1)
			;
		else
		{
			printf("고도리입니다!\n");
			player->hcard->godori_counted = 1;
			player->score += 5;
		}
	}
	return;
	
}

void HelpPrint()
{
	;
	printf(" \n\n\ng(o) : 고 (자기 turn에 점수가 났고 3점 이상일 때 할 수 있음, )\ns(top) : 스톱 (자기 turn에 점수가 났고 3점 이상일 때 할 수 있음, 이번 판을 끝냄)\ne(xit) : exit (프로그램 끝내기) \nb(alance) : 게이머의 잔고 보기\nh(elp) : 각 키에 대한 설명 보기\n1~7 : 낼 화투 선택\n1~2 : 화투를 냈는데, 깔린 화투 중 무늬는 같지만 값이 다른 것이 있을 때 선택\n9 : 9 십을 피로 또는 십으로 이동 (토글), 디폴트로는 피로 함, 각 판에서 한번만 할 수 있음.\nsave : 현재 상태를 파일에 저장 (단, 확인 가능하도록 텍스트 형태로 저장해야 함)\nload : 파일에 저장된 상태를 읽어서 계속 게임 진행\n\n\n");
}


int ChongTongCheck(Player* a, Player* b, Player* c)
{
	int card=0;
	int i;
	Dcard* dcard_save[3] = { a->dcard, b->dcard, c->dcard };
	for(i=0; i<3;)
	{
		while(dcard_save[i]->next->card != 0)
		{
			if((dcard_save[i]->card / 10) == (dcard_save[i]->next->card / 10))
				++card;
			else
				card=0;

			if(card==3)
			{	
				if(i==0)
				{
					printf("플레이어 A님의 총통입니다!\n");
					a->score = 10;
					a->winner=1;
					return -1;
				}
				else if (i==1)
				{
					printf("플레이어 B님의 총통입니다!\n");
					b->score = 10;
					b->winner=1;
					return -1;
				}
				else
				{
					printf("플레이어 C님의 총통입니다!\n");
					c->score = 10;
					c->winner=1;
					return -1;
				}
			}
			dcard_save[i] = dcard_save[i]->next;
		}
		i++;
	}
	return 0;
}

void Gu_Ten_Exchange(Hcard* hcard)
{
	if(hcard->card == 95)
	{
		hcard->card = 91;
		return;
	}
	else
		Gu_Ten_Exchange(hcard->next);
}

void WinnerFinalScore(Player *player)
{
	if(player->mungtungguri == 1)
		player->score *= 2;
	if(player->heundel == 1)
		player->score *= 2;
	if(player->go >= 1)
	{
		if(player->go == 1)
			player->score += 1;
		else if(player->go == 2)
			player->score += 2;
		else if (player->go == 3)
			player->score *= 2;
		else if(player->go == 4)
			player->score *= 4;
		else if(player->go == 5)
			player->score *= 5;
	}
	if(nagari==1)
		player->score *= 2;
}

void MoneyCalculate(Player* winner, Player *loser1, Player *loser2)
{
	printf("winner 스코어: %d", winner->score);
	if(loser1->gobak== 1) // 고박 확인
	{
			winner->money += (winner->score) * 200;
			loser1->money -= (winner->score) * 200;
		if((loser1->hcard->pi_num_counted <= 5 && loser1->hcard->pi_num_counted!=0)) // 피박 확인
		{
			winner->money += (winner->score) * 100;
			loser1->money -= (winner->score) * 100;
		}
		if((loser2->hcard->pi_num_counted <= 5 && loser2->hcard->pi_num_counted!=0))
		{
			winner->money += (winner->score) * 100;
			loser1->money -= (winner->score) * 100;
		}
	}
	else if(loser2->gobak == 1)
	{
		winner->money += (winner->score) * 200;
		loser2->gobak -= (winner->score) * 200;
		if((loser1->hcard->pi_num_counted <= 5 && loser1->hcard->pi_num_counted!=0))
		{
			winner->money += (winner->score) * 100;
			loser2->money -= (winner->score) * 100;
		}
		if((loser2->hcard->pi_num_counted <= 5 && loser2->hcard->pi_num_counted!=0))
		{
			winner->money += (winner->score) * 100;
			loser2->money -= (winner->score) * 100;
		}
	}
	else
	{
		winner->money += (winner->score) * 200;
		loser1->money -= (winner->score) * 100;
		loser2->money -= (winner->score) * 100;
		if(loser1->hcard->pi_num_counted <= 5 && loser1->hcard->pi_num_counted!=0)
		{
			printf("피박입니다!\n");
			winner->money += (winner->score) * 100;
			loser1->money -= (winner->score) * 100;
		}
		if(loser2->hcard->pi_num_counted <= 5 && loser2->hcard->pi_num_counted!=0)
		{
			printf("피박입니다!\n");
			winner->money += (winner->score) * 100;
			loser2->money -= (winner->score) * 100;
		}
	}
}

void EndSequence(Player *a, Player *b, Player *c)
{
	CardVerify(a);
	CardVerify(b);
	CardVerify(c);
	if(a->winner == 1)
	{
		WinnerFinalScore(a);
		MoneyCalculate(a,b,c);
	}
	else if(b->winner == 1)
	{
		WinnerFinalScore(b);
		MoneyCalculate(b,a,c);
	}
	else if(c->winner == 1)
	{
		WinnerFinalScore(c);
		MoneyCalculate(c,a,b);
	}
}

void Save_Sequence(Player* a, Player* b, Player *c, Ucard* ucard, Gcard* gcard)
{
	FILE * fp = fopen("save.txt", "wt");
	Hcard* hcard = a->hcard;
	Dcard* dcard = a->dcard;
	Hcard* hcard_pointer_save = a->hcard;
	Dcard* dcard_pointer_save= a->dcard;
	Ucard* ucard_pointer_save = ucard;
	Gcard* gcard_pointer_save = gcard;
	ucard_save=0;
	gcard_save=0;
	a_dcard_save=0;
	a_hcard_save=0;
	b_dcard_save=0;
	b_hcard_save=0;
	c_dcard_save=0;
	c_hcard_save=0;

	fprintf(fp, "나가리: %d ",nagari);
	while(ucard->card != 0)
	{
		ucard_save++;
		ucard=ucard->next;
	}
	fprintf(fp, "화투 더미 개수: %d ", ucard_save);
	ucard = ucard_pointer_save;
	fprintf(fp, "화투 더미: ");
	while(ucard ->card != 0)
	{
		fprintf(fp, "%d ", ucard->card);
		ucard = ucard->next;
	}

	while(gcard->card != 0)
	{
		gcard_save++;
		gcard=gcard->next;
	}
	fprintf(fp, "깔린 패 개수: %d ", gcard_save);
	gcard = gcard_pointer_save;
	fprintf(fp, "깔린 패: ");
	while(gcard->card  != 0)
	{
		fprintf(fp, "%d ", gcard->card);
		gcard = gcard->next;
	}

	while(dcard->card != 0)
	{
		a_dcard_save++;
		dcard=dcard->next;
	}
	fprintf(fp, "플레이어A 든 패 개수: %d ", a_dcard_save);
	dcard = dcard_pointer_save;
	fprintf(fp, "플레이어A 든 패: ");
	while(dcard->card  != 0)
	{
		fprintf(fp, "%d ", dcard->card);
		dcard = dcard->next;
	}
	while(hcard->card != 0)
	{
		a_hcard_save++;
		hcard=hcard->next;
	}
	hcard = hcard_pointer_save;
	fprintf(fp, "플레이어A 먹은 패 개수: %d ", a_hcard_save);
	fprintf(fp, "플레이어A 먹은 패: ");
	while(hcard->card != 0)
	{
		fprintf(fp, "%d ", hcard->card);
		hcard = hcard->next;
	}
	fprintf(fp, "플레이어A 고: %d ", a->go);
	fprintf(fp, "플레이어A 멍텅구리: %d ", a->mungtungguri);
	fprintf(fp, "플레이어A 흔들: %d ", a->heundel);
	fprintf(fp, "플레이어A 차례: %d ", a->order);
	fprintf(fp, "플레이어A 점수: %d ", a->score);
	fprintf(fp, "플레이어A 잔액: %d ", a->money);

	dcard = b->dcard;
	hcard = b->hcard;
	dcard_pointer_save = b->dcard;
	hcard_pointer_save = b->hcard;
	while(dcard->card != 0)
	{
		b_dcard_save++;
		dcard=dcard->next;
	}
	fprintf(fp, "플레이어B 든 패 개수: %d ", b_dcard_save);
	dcard = dcard_pointer_save;
	fprintf(fp, "플레이어B 든 패: ");
	while(dcard->card  != 0)
	{
		fprintf(fp, "%d ", dcard->card);
		dcard = dcard->next;
	}
	while(hcard->card != 0)
	{
		b_hcard_save++;
		hcard=hcard->next;
	}
	hcard = hcard_pointer_save;
	fprintf(fp, "플레이어B 먹은 패 개수: %d ", b_hcard_save);
	fprintf(fp, "플레이어B 먹은 패: ");
	while(hcard->card != 0)
	{
		fprintf(fp, "%d ", hcard->card);
		hcard = hcard->next;
	}
	fprintf(fp, "플레이어B 고: %d ", b->go);
	fprintf(fp, "플레이어B 멍텅구리: %d ", b->mungtungguri);
	fprintf(fp, "플레이어B 흔들: %d ", b->heundel);
	fprintf(fp, "플레이어B 차례: %d ", b->order);
	fprintf(fp, "플레이어B 점수: %d ", b->score);
	fprintf(fp, "플레이어B 잔액: %d ", b->money);

	dcard = c->dcard;
	hcard = c->hcard;
	dcard_pointer_save = c->dcard;
	hcard_pointer_save = c->hcard;
	while(dcard->card != 0)
	{
		c_dcard_save++;
		dcard=dcard->next;
	}
	fprintf(fp, "플레이어C 든 패 개수: %d ", c_dcard_save);
	dcard = dcard_pointer_save;
	fprintf(fp, "플레이어C 든 패: ");
	while(dcard->card  != 0)
	{
		fprintf(fp, "%d ", dcard->card);
		dcard = dcard->next;
	}
	while(hcard->card != 0)
	{
		c_hcard_save++;
		hcard=hcard->next;
	}
	hcard = hcard_pointer_save;
	fprintf(fp, "플레이어C 먹은 패 개수: %d ", c_hcard_save);
	fprintf(fp, "플레이어C 먹은 패: ");
	while(hcard->card != 0)
	{
		fprintf(fp, "%d ", hcard->card);
		hcard = hcard->next;
	}
	fprintf(fp, "플레이어C 고: %d ", c->go);
	fprintf(fp, "플레이어C 멍텅구리: %d ", c->mungtungguri);
	fprintf(fp, "플레이어C 흔들: %d ", c->heundel);
	fprintf(fp, "플레이어C 차례: %d ", c->order);
	fprintf(fp, "플레이어C 점수: %d ", c->score);
	fprintf(fp, "플레이어C 잔액: %d ", c->money);
	fclose(fp);
}

void Load_Sequence(Player* a, Player* b, Player *c, Ucard* ucard, Gcard* gcard) // 여기 고쳐야함
{
	int ret;
	FILE* fp;
	Hcard* hcard_save = a->hcard;
	Dcard* dcard_save = a->dcard;
	Ucard* initial_ucard = ucard;
	Gcard* initial_gcard = gcard;
	if((fp= fopen("save.txt", "rt")) == NULL)
	{
			printf("파일을 열 수 없습니다.\n");
			return;
	}
	else
		printf("파일 열기에 성공했습니다.\n");

	ret=fscanf(fp, "나가리: %d ", &nagari);
	if(ret==EOF)
		printf("파일 읽기에 실패했습니다. \n");
	else
		printf("파일 읽기에 성공했습니다.\n");
	fscanf(fp, "화투 더미 개수: %d ", &ucard_save);
	printf("\n\n\n화투 더미 개수: %d \n\n\n", ucard_save);
	if(ucard_save==0)
	{
		ucard->card=0;
		ucard->next=NULL;
	}
	fscanf(fp, "화투 더미: ");
	while(ucard_save>0)
	{
		fscanf(fp, "%d ", &(ucard->card));
		ucard->next = (Ucard*)calloc(1, sizeof(Ucard));
		ucard = ucard->next;
		ucard_save--;
	}
	ucard_save=0;
	fscanf(fp, "깔린 패 개수: %d ", &gcard_save);
	printf("\n\n\n깔린 패 개수: %d \n\n\n", gcard_save);
	if(gcard_save==0)
	{
		gcard->card=0;
		gcard->next=NULL;
	}
	fscanf(fp, "깔린 패: ");
	while(gcard_save > 0)
	{
		fscanf(fp, "%d ", &(gcard->card));
		gcard->next = (Gcard*)calloc(1, sizeof(Gcard)); 
		gcard = gcard->next;
		gcard_save--;
	}
	gcard_save=0;
	gcard_State(initial_gcard);
	fscanf(fp, "플레이어A 든 패 개수: %d ", &a_dcard_save);
	printf("\n플레이어A 든 패 개수 %d", a_dcard_save);
	if(a_dcard_save==0)
	{
		dcard_save->card=0;
		dcard_save->next=NULL;
	}
	fscanf(fp, "플레이어A 든 패: ");
	while(a_dcard_save > 0)
	{
		fscanf(fp, "%d ", &(dcard_save->card));
		dcard_save->next = (Dcard*)calloc(1, sizeof(Dcard));
		dcard_save = dcard_save->next;
		a_dcard_save--;
	}
	a_dcard_save=0;
	dcard_State(a->dcard);
	fscanf(fp, "플레이어A 먹은 패 개수: %d ", &a_hcard_save);
	printf("\n플레이어A 먹은 패 개수 %d", a_hcard_save);
	if(a_hcard_save==0)
	{
		hcard_save->card=0;
		hcard_save->next=NULL;
	}
	fscanf(fp, "플레이어A 먹은 패 : ");
	while(a_hcard_save > 0)
	{
		fscanf(fp, "%d ", &(hcard_save->card));
		hcard_save->next = (Hcard*)calloc(1, sizeof(Hcard));
		hcard_save = hcard_save->next;
		a_hcard_save--;
	}
	a_hcard_save=0;
	hcard_State(a->hcard);
	fscanf(fp, "플레이어A 고: %d ", &(a->go));
	printf("플레이어A 고: %d", a->go);
	fscanf(fp, "플레이어A 멍텅구리: %d ", &(a->mungtungguri));
	fscanf(fp, "플레이어A 흔들: %d ", &(a->heundel));
	printf("플레이어A 흔들: %d ", a->heundel);
	fscanf(fp, "플레이어A 차례: %d ", &(a->order));
	printf("플레이어 A 차례: %d ", a->order);
	fscanf(fp, "플레이어A 점수: %d ", &(a->score));
	printf("플레이어A 점수: %d ", a->score);
	fscanf(fp, "플레이어A 잔액: %d ", &(a->money));
	printf("플레이어A 잔액: %d\n", a->money);

	dcard_save = b->dcard;
	hcard_save = b->hcard;
	fscanf(fp, "플레이어B 든 패 개수: %d ", &b_dcard_save);
	printf("\n플레이어B 든 패 개수 %d", b_dcard_save);
	if(b_dcard_save==0)
	{
		dcard_save->card=0;
		dcard_save->next=NULL;
	}
	fscanf(fp, "플레이어B 든 패: ");
	while(b_dcard_save > 0)
	{
		fscanf(fp, "%d ", &(dcard_save->card));
		dcard_save->next = (Dcard*)calloc(1, sizeof(Dcard));
		dcard_save = dcard_save->next;
		b_dcard_save--;
	}
	b_dcard_save=0;
	dcard_State(b->dcard);
	fscanf(fp, "플레이어B 먹은 패 개수: %d ", &b_hcard_save);
	printf("\n플레이어B 먹은 패 개수 %d", b_hcard_save);
	if(b_hcard_save==0)
	{
		hcard_save->card=0;
		hcard_save->next=NULL;
	}
	fscanf(fp, "플레이어B 먹은 패: ");
	while(b_hcard_save > 0)
	{
		fscanf(fp, "%d ", &(hcard_save->card));
		hcard_save->next = (Hcard*)calloc(1, sizeof(Hcard));
		hcard_save = hcard_save->next;
		b_hcard_save--;
	}
	b_hcard_save=0;
	hcard_State(b->hcard);
	fscanf(fp, "플레이어B 고: %d ", &(b->go));
	printf("플레이어B 고: %d ", b->go);
	fscanf(fp, "플레이어B 멍텅구리: %d ", &(b->mungtungguri));
	printf("플레이어B 멍텅구리: %d ", b->mungtungguri);
	fscanf(fp, "플레이어B 흔들: %d ", &(b->heundel));
	printf("플레이어B 흔들: %d ", b->heundel);
	fscanf(fp, "플레이어B 차례: %d ", &(b->order));
	printf("플레이어B 차례: %d ", b->order);
	fscanf(fp, "플레이어B 점수: %d ", &(b->score));
	printf("플레이어B 점수: %d ", b->score);
	fscanf(fp, "플레이어B 잔액: %d ", &(b->money));
	printf("플레이어B 잔액: %d ", b->money);

	dcard_save = c->dcard;
	hcard_save = c->hcard;
	fscanf(fp, "플레이어C 든 패 개수: %d ", &c_dcard_save);
	printf("\n플레이어C 든 패 개수 %d", c_dcard_save);
	if(c_dcard_save==0)
	{
		dcard_save->card=0;
		dcard_save->next=NULL;
	}
	fscanf(fp, "플레이어C 든 패: ");
	while(c_dcard_save > 0)
	{
		fscanf(fp, "%d ", &(dcard_save->card));
		dcard_save->next = (Dcard*)calloc(1, sizeof(Dcard));
		dcard_save = dcard_save->next;
		c_dcard_save--;
	}
	c_dcard_save=0;
	dcard_State(c->dcard);
	fscanf(fp, "플레이어C 먹은 패 개수: %d ", &c_hcard_save);
	printf("\n플레이어C 먹은 패 개수 %d", c_hcard_save);
	if(c_hcard_save==0)
	{
		hcard_save->card=0;
		hcard_save->next=NULL;
	}
	fscanf(fp, "플레이어C 먹은 패: ");
	while(c_hcard_save > 0)
	{
		fscanf(fp, "%d ", &(hcard_save->card));
		hcard_save->next = (Hcard*)calloc(1, sizeof(Hcard));
		hcard_save = hcard_save->next;
		c_hcard_save--;
	}
	c_hcard_save=0;
	hcard_State(c->hcard);
	fscanf(fp, "플레이어C 고: %d ", &(c->go));
	printf("플레이어C 고: %d ", c->go);
	fscanf(fp, "플레이어C 멍텅구리: %d ", &(c->mungtungguri));
	printf("플레이어C 멍텅구리: %d ", c->mungtungguri);	
	fscanf(fp, "플레이어C 흔들: %d ", &(c->heundel));
	printf("플레이어C 흔들: %d ", c->heundel);
	fscanf(fp, "플레이어C 차례: %d ", &(c->order));
	printf("플레이어C 차례 : %d ", c->order);
	fscanf(fp, "플레이어C 점수: %d ", &(c->score));
	printf("플레이어C 점수: %d ", c->score);
	fscanf(fp, "플레이어C 잔액: %d ", &(c->money));
	printf("플레이어C 잔액: %d ", c->money);
	fclose(fp);
}	

void AllReset(Ucard* ucard,Gcard* gcard,Player* a, Player* b, Player*c)
{
	Gcard* gcard_save;
	Ucard* ucard_save;
	Hcard* hcard = a->hcard;
	Dcard* dcard = a->dcard;
	Hcard* hcard_save;
	Dcard* dcard_save;
	game_over=0;
	while(gcard->card != 0)
	{
		gcard->card =0;
		gcard->first_card=0;
		gcard->total_cards=0;
		gcard_save = gcard;
		gcard = gcard->next;
		free(gcard_save);
	}
	while(ucard->card != 0)
	{
		ucard->card =0;
		ucard->card_num=0;
		ucard_save = ucard;
		ucard = ucard->next;
		free(ucard_save);
	}
	while(hcard->card != 0)
	{
		hcard-> card=0;
		hcard-> gu_ten=0;
		hcard-> hongdan=0;
		hcard-> chungdan=0;
		hcard-> chodan=0;
		hcard-> bigwang=0;
		hcard-> godori=0;
		hcard-> pi_num_counted=0;
		hcard-> ten_num_counted=0;
		hcard-> five_num_counted=0;
		hcard-> kwang_num_counted=0;
		hcard-> hongdan_counted=0;
		hcard-> chungdan_counted=0;
		hcard-> chodan_counted=0;
		hcard-> godori_counted=0;
		hcard->card =0;
		hcard_save = hcard;
		hcard = hcard->next;
		free(hcard_save);
	}
	while(dcard->card != 0)
	{
		dcard->card=0;
		dcard->card_num=0;
		dcard_save = dcard;
		dcard = dcard->next;
		free(dcard_save);
	}
	hcard = b->hcard;
	dcard = b->dcard;
	while(hcard->card!= 0)
	{
		hcard-> card=0;
		hcard-> gu_ten=0;
		hcard-> hongdan=0;
		hcard-> chungdan=0;
		hcard-> chodan=0;
		hcard-> bigwang=0;
		hcard-> godori=0;
		hcard-> pi_num_counted=0;
		hcard-> ten_num_counted=0;
		hcard-> five_num_counted=0;
		hcard-> kwang_num_counted=0;
		hcard-> hongdan_counted=0;
		hcard-> chungdan_counted=0;
		hcard-> chodan_counted=0;
		hcard-> godori_counted=0;
		hcard->card =0;
		hcard_save = hcard;
		hcard = hcard->next;
		free(hcard_save);
	}
	while(dcard->card != 0)
	{
		dcard->card=0;
		dcard->card_num=0;
		dcard_save = dcard;
		dcard = dcard->next;
		free(dcard_save);
	}
	hcard = c->hcard;
	dcard = c->dcard;
	while(hcard->card != 0)
	{
		hcard-> card=0;
		hcard-> gu_ten=0;
		hcard-> hongdan=0;
		hcard-> chungdan=0;
		hcard-> chodan=0;
		hcard-> bigwang=0;
		hcard-> godori=0;
		hcard-> pi_num_counted=0;
		hcard-> ten_num_counted=0;
		hcard-> five_num_counted=0;
		hcard-> kwang_num_counted=0;
		hcard-> hongdan_counted=0;
		hcard-> chungdan_counted=0;
		hcard-> chodan_counted=0;
		hcard-> godori_counted=0;
		hcard->card =0;
		hcard_save = hcard;
		hcard = hcard->next;
		free(hcard_save);
	}
	while(dcard->card != 0)
	{
		dcard->card=0;
		dcard->card_num=0;
		dcard_save = dcard;
		dcard = dcard->next;
		free(dcard_save);
	}
	a->bbuk=0;
	a->ChongTong=0;
	a->go=0;
	a->gobak=0;
	a->heundel=0;
	a->mungtungguri=0;
	a->order=1;
	a->score=0;
	a->winner=0;

	b->bbuk=0;
	b->ChongTong=0;
	b->go=0;
	b->gobak=0;
	b->heundel=0;
	b->mungtungguri=0;
	b->order=0;
	b->score=0;
	b->winner=0;

	c->bbuk=0;
	c->ChongTong=0;
	c->go=0;
	c->gobak=0;
	c->heundel=0;
	c->mungtungguri=0;
	c->order=0;
	c->score=0;
	c->winner=0;
}

void my_bbuk(Player* player1, Player* player2, Player* player3) // 카드 주는 함수
{
	Hcard* player_hcard_save1;
	Hcard* player_hcard_save2=NULL;

	player_hcard_save1 = player2->hcard;
	while(player_hcard_save1->card != 0)
	{
		if(((player_hcard_save1->card) % 10) == 3 || ((player_hcard_save1->card) % 10) == 4)
		{
			HcardAdd(player1->hcard, player_hcard_save1->card);
			HcardMinus(player_hcard_save1);
			player_hcard_save2=NULL;
			break;
		}
		else if(player_hcard_save1->card % 10 == 5)
				player_hcard_save2 = player_hcard_save1; // 쌍피 저장

		player_hcard_save1 = player_hcard_save1->next;

		if(player_hcard_save1->card == 0) // 피는 없지만 쌍피는 있을 때
		{
			if(player_hcard_save2 != NULL)
			{
				HcardAdd(player1->hcard, player_hcard_save2->card);
				HcardMinus(player_hcard_save2);
				player_hcard_save2 = NULL;
				break;
			}
		}
	}

	player_hcard_save1 = player3->hcard;
	player_hcard_save2=NULL;

	while(player_hcard_save1->card != 0)
	{
		if(((player_hcard_save1->card) % 10) == 3 || ((player_hcard_save1->card) % 10) == 4)
		{
			HcardAdd(player1->hcard, player_hcard_save1->card);
			HcardMinus(player_hcard_save1);
			player_hcard_save2=NULL;
			break;
		}
		else if(player_hcard_save1->card % 10 == 5)
			player_hcard_save2 = player_hcard_save1;

		player_hcard_save1 = player_hcard_save1->next;
		if(player_hcard_save1->card == 0)
		{
			if(player_hcard_save2 != NULL)
			{
				HcardAdd(player1->hcard, player_hcard_save2->card);
				HcardMinus(player_hcard_save2);
				player_hcard_save2 = NULL;
				break;
			}
			player_hcard_save2 = NULL;
		}	
	}
}

void Heundel(Player* player)
{
	int dsave[48];
	int n=0;
	Dcard* dcard = player->dcard;
	while(dcard->card != 0)
	{
		dsave[n]=dcard->card;
		if(n>=2)
		{
			if(player->heundel != 1)
			{
				if((dsave[n-2]/10) == (dsave[n-1]/10) ? ( (dsave[n-1]/10) == (dsave[n]/10) ? 1 : 0 ) : 0)
				{
					player->heundel = 1;
					if(player==&a)
					{
						printf("플레이어A가 흔들었습니다!\n");
						printf("A 흔들: %d\n", a.heundel);
					}
					else if (player==&b)
					{
						printf("플레이어B가 흔들었습니다!\n");
						printf("B 흔들: %d\n", b.heundel);
					}
					else if (player==&c)
					{
						printf("플레이어C가 흔들었습니다!\n");
						printf("C 흔들: %d\n", c.heundel);
					}
				}
			}
		}
		dcard = dcard->next;
		n++;
	}
}

