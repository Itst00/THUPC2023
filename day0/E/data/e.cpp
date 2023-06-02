#include<bits/stdc++.h>
//#pragma gcc optimize(3)
#define gc getchar()
//char buf[100000],*buff = buf + 100000;
//#define gc ((buff == buf + 100000 ? (fread(buf,1,100000,stdin),buff = buf) : 0),*(buff++))
#define pc putchar
//char bfu[10000000],*bfuu = bfu;
//#define pc(x) (*(bfuu++) = x)
#define li long long
#define uli unsigned li
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define pii pair<int,int>
#define md int mid = l + r >> 1
#define ls q << 1
#define rs q << 1 | 1
#define ln ls,l,mid
#define rn rs,mid + 1,r
using namespace std;
//const int mo = 998244353;
//const int mo = 1000000007;
inline li read(){
	li x = 0;
	int y = 0,c = gc;
	while(c < '0' || c > '9') y = c,c = gc;
	while(c >= '0' && c <= '9') x = x * 10 + c - '0',c = gc;
	return y == '-' ? -x : x;
}
inline void prt(li x){
	if(x >= 10) prt(x / 10);
	pc(x % 10 + '0');
}
inline void print(li x){
	if(x < 0) pc('-'),x = -x;
	prt(x);
}
inline void file(char *s){
	char c[50];
	sprintf(c,"%s.in",s);
	freopen(c,"r",stdin);
	sprintf(c,"%s.out",s);
	freopen(c,"w",stdout);
}
li s1 = 19260817,s2 = 23333333,s3 = 998244853,srd;
inline li rd(){
	return srd = (srd * s1 + s2 + rand()) % s3;
}
#define db long double
//��������
#define SOUTH 0
#define NORTH 1
string team_name[2] = {"South","North"};
//��������
#define WEAK 0
#define AVERAGE 1
#define STRONG 2
string skill_name[3] = {"Weak","Average","Strong"};
//��������
#define B 0
#define G 1
#define M 2
//��������
#define BASIC 0
#define SPECIAL 1
#define SKILL 2
inline db read_db(){
	int x = read(),y = read();
	return x * powl(10,y);
}

struct Player{
	int team;//����
	int id;//���
	int type;//����
	int lv;//�ȼ�
	int hp;//����ֵ
	int maxhp;//��������
	db atk;//��������ָ��
	db def;//��������ָ��
	int skill_lv;//�������ܵȼ�
	int passiveskl_lv;//�������ܵȼ�
	int weapon_type;//��������
	db weapon_atk;//����������
	db average_skill_effect;//average���������ܵ�Ч����ÿ�غϽ���ʱ�������Ҫ��Ѫ
	int average_skill_effect_left;//��Ѫdebuff���ж��ٸ��غ�
	db team_atk;//���鹥���ӳ�
	db team_def;//��������ӳ�
	db skill_additional;//���ܼӳ�
	void input(int _team,int _id){
		team = _team;
		id = _id;
		string s;
		cin>>s;
		type = (s.size() == 4 ? WEAK : s.size() == 7 ? AVERAGE : STRONG);
		lv = read();
		maxhp = hp = read();
		atk = read_db();
		def = read_db();
		skill_lv = read();
		passiveskl_lv = read();
		cin>>s;
		weapon_type = (s[0] == 'B' ? B : s[0] == 'G' ? G : M);
		weapon_atk = read_db();
		average_skill_effect = 0;
		average_skill_effect_left = 0;
		skill_additional = 1;
	}
	inline bool is_null(){return id == 0;}//�ǲ���null
	inline bool is_full_hp(){return hp == maxhp;}//�ǲ�����Ѫ
	inline bool is_died(){return hp <= 0;}//�ǲ�������
	inline int change_hp(int x){//��Ѫ���Ѫ
		int oldhp = hp;
		hp = max(0,min(hp + x,maxhp));
		return hp - oldhp;
	}
	void out(){
		cerr<<"team "<<team_name[team]<<" id "<<id<<":"<<endl;
		cerr<<"type: "<<type<<" lv: "<<lv<<" hp: "<<hp<<" maxhp: "<<maxhp<<endl;
		cerr<<"atk: "<<atk<<" def: "<<def<<" skill_lv: "<<skill_lv<<" passiveskl_lv: "<<passiveskl_lv<<endl;
		cerr<<"weapon_type: "<<weapon_type<<" weapon_atk: "<<weapon_atk<<" average_skill_effect: "<<average_skill_effect<<" average_skill_effect_left: "<<average_skill_effect_left<<endl;
		cerr<<"team_atk: "<<team_atk<<" team_def: "<<team_def<<" skill_additional: "<<skill_additional<<endl<<endl;
	}
}a[2][20],null_player;
int n[2],t;
int lst[2];//ÿ��������һ���ж�����
struct Attack{
	int id;//�غϱ��
	int type;//��������
	int target;//����Ŀ��
	int atkpos;//������λ
	int ddgpos;//������λ
	void input(int _id){
		id = _id;
		string s;
		cin>>s;
		target = read();
		if(s.size() == 5){
			type = SKILL;
		}
		else{
			type = (s.size() == 11 ? BASIC : SPECIAL);
			atkpos = read();
			ddgpos = read();
		}
	}
};

inline int chk_team_win(){//�Ƿ��ж���Ӯ��
	int ans = 2;
	for(int i = 0;i < 2;++i){
		bool flag = 1;
		for(int j = 1;j <= n[i];++j) if(a[i][j].hp > 0){
			flag = 0;
			break;
		}
		if(flag){//��Ӫiȫ����
			ans = 1 - i;
			return ans;
		}
	}
	return ans;
}

int west_to_east[] = {0,5,3,1,2,4,6};//����������˳��Ҳ������ı��˳��
int west_to_east_pos[] = {0,3,4,2,5,1,6};//����������˳��ÿ����ŵ��˵�λ��
struct Output{
	void output_recovery(Player &x,int heal){//x��Ѫheal��heal�Ǳ�Ӧ���ӵ�Ѫ����Ҫ������
		//if(!heal) return;
		if(x.is_full_hp()) return;
		x.change_hp(heal);
		cout<<team_name[x.team]<<" "<<x.id<<" recovered +"<<heal<<" hp -> "<<x.hp<<"/"<<x.maxhp<<endl;
	}
	void output_dmg(Player &x,Player &y,int dmg){//x��y���dmg���˺�
		//if(!dmg) return;
		y.change_hp(-dmg);
		cout<<team_name[y.team]<<" "<<y.id<<" took "<<dmg<<" damage from "<<team_name[x.team]<<" "<<x.id<<" -> "<<y.hp<<"/"<<y.maxhp<<endl;
	}
	void output_dmg_skill(Player &x,int dmg){//x���ڼ����ܵ�dmg���˺�
		//if(!dmg) return;
		x.change_hp(-dmg);
		cout<<team_name[x.team]<<" "<<x.id<<" took "<<dmg<<" damage from skill -> "<<x.hp<<"/"<<x.maxhp<<endl;
	}
	void output_apply_skill(Player &x,Player &y){//x��yʹ�ü���
		cout<<team_name[x.team]<<" "<<x.id<<" applied "<<skill_name[x.type]<<" skill to "<<team_name[y.team]<<" "<<y.id<<endl;
	}
	void output_all(){
		for(int i = 1;i >= 0;--i){
			cout<<team_name[i]<<":";
			for(int j = 1;j <= 6;++j){
				int id = west_to_east[j];
				if(id > n[i]) continue;
				cout<<" "<<a[i][id].hp<<"/"<<a[i][id].maxhp;
			}
			cout<<endl;
		}
		cout<<endl;
	}
	void end_game(){//������Ϸ����
		int res = chk_team_win();
		if(res == 2) return;
		cout<<"Team "<<team_name[res]<<" won."<<endl;
	}
}output;

Player& get_west(Player &x){//���һ�������ߵĵ�һ���ˣ������ڷ���null_player
	int team = x.team,id = west_to_east_pos[x.id];
	for(int i = id - 1;i;--i){
		if(west_to_east[i] > n[team]) continue;
		Player &y = a[team][west_to_east[i]];
		if(!y.is_died()) return y;		
	}
	return null_player;
}
Player& get_east(Player &x){//���һ���˶��ߵĵ�һ���ˣ������ڷ���null_player
	int team = x.team,id = west_to_east_pos[x.id];
	for(int i = id + 1;i <= 6;++i){
		if(west_to_east[i] > n[team]) continue;
		Player &y = a[team][west_to_east[i]];
		if(!y.is_died()) return y;		
	}
	return null_player;
}


Player& get_now_player(int id){//��id���غϣ���ȡ��ǰ�ж�����
	assert(chk_team_win() == 2);
	//output.output_all();
	int team = id % 2;
	int &i = lst[team];
	//cerr<<team<<" "<<i<<":::"<<n[team]<<endl;
	i = i % n[team] + 1;
	while(a[team][i].is_died()) i = i % n[team] + 1;
	return a[team][i];
}

db weak_pskill_val[] = {0,1.3l,1.6l,1.9l,2.2l,2.5l};//weak�������ܵĲ�������ֵ�ǰٷֱ�
void get_weak_pskill(int team){//�غϿ�ʼʱ������weak�ı�������
	for(int i = 1;i <= n[team];++i){
		Player &x = a[team][i];
		if(x.is_died()) continue;
		db val = 0;
		bool flag = 0;
		for(int j = 1;j <= n[team];++j){
			Player &y = a[team][j];
			if(y.is_died() || y.type != WEAK || y.passiveskl_lv == 0) continue;
			val += weak_pskill_val[y.passiveskl_lv];
			flag = 1;
		}
		//cerr<<team<<" "<<i<<":"<<endl;
		//x.out();
		if(flag) output.output_recovery(x,min(val,5.0l) / 100.0l * x.maxhp);
	}
}

void get_average_skill_effect(int team){//�غϽ���ʱ������average���������ܵĵ�ѪЧ��
	for(int i = 1;i <= n[team];++i){
		Player &x = a[team][i];
		if(x.is_died() || !x.average_skill_effect) continue;
		//x.out();
		output.output_dmg_skill(x,x.average_skill_effect / 100.0l * x.maxhp);
		--x.average_skill_effect_left;
		if(!x.average_skill_effect_left) x.average_skill_effect = 0;//��Ѫdebuff��ʱ����
	}
}

db pskill_val[] = {0,0.01l,0.02l,0.03l,0.04l,0.05l};//�������ֱ������ܵĲ���
void get_pskill(){//�����漰������ָ��������pskill
	for(int team = 0;team < 2;++team){
		for(int i = 1;i <= n[team];++i){
			Player &x = a[team][i];
			if(x.is_died()) continue;
			db atk_val = 0,def_val = 0;
			for(int j = 1;j <= n[team];++j){
				Player &y = a[team][j];
				if(y.is_died()) continue;
				if(y.type == AVERAGE) def_val += pskill_val[y.passiveskl_lv];//�����ӳ�
				else if(y.type == STRONG) atk_val += pskill_val[y.passiveskl_lv];//�����ӳ�
			}
			x.team_atk = 1 + min(0.1l,atk_val);
			x.team_def = 1 + min(0.1l,def_val);
		}
	}

}

db fwjc[] = {1.25l,1,0.75l,0,0.75l,1};//��λ�ӳɲ���
db get_fwjc(int atkpos,int ddgpos){//���㷽λ�ӳ�
	return fwjc[(atkpos - ddgpos + 6) % 6];
}
db zzkzjc[3][3] = {{1,0.9l,1.1l},{1.1l,1,0.9l},{0.9l,1.1l,1}};//������Ƽӳɲ���
db get_zzkzjc(Player &plr,Player &tar){//����������Ƽӳ�
	return zzkzjc[plr.type][tar.type];
}
void attack(Player &plr,Player &tar,db basic_val,int atkpos,int ddgpos){//�������㣬basic_val�ǻ�������ǿ��
	//���㡸����ǿ�ȡ����ڡ���������ǿ�ȡ����Է��𡸹��������˵ġ����ܼӳɡ����Զ���ġ������ӳɡ����ԡ�������Ƽӳɡ����ԡ���λ�ӳɡ�
	db true_val = basic_val * plr.skill_additional * plr.team_atk * get_zzkzjc(plr,tar) * get_fwjc(atkpos,ddgpos);
	output.output_dmg(plr,tar,true_val / (tar.def * tar.team_def));
	//cout<<basic_val<<" "<<true_val<<" "<<tar.def<<" "<<tar.team_def<<": "<<true_val / (tar.def * tar.team_def)<<endl;
	get_pskill();//Ҫע��ѱ��˸���֮����±������ܵ�Ч�� 
}

Player& get_target(Player &x,Attack atk){//���Ŀ���Ա
	//cerr<<x.team<<" "<<atk.target<<":::"<<endl;
	//a[x.team][atk.target].out();
	//a[!x.team][atk.target].out();
	if(atk.type == SKILL && x.type != AVERAGE) return a[x.team][atk.target];//������Ա
	return a[!x.team][atk.target];//�Է���Ա
}

void basic_attack(Player &plr,Player &tar,int atkpos,int ddgpos){//������ͨ����
	attack(plr,tar,plr.atk * plr.weapon_atk,atkpos,ddgpos);
	plr.skill_additional = 1;
}

void special_attack(Player &plr,Player &tar,int atkpos,int ddgpos){//�������⹥��
	if(plr.weapon_type == B){
		attack(plr,tar,plr.atk * plr.weapon_atk * 1.25l,atkpos,ddgpos);
	}
	else if(plr.weapon_type == G){
		Player &wst = get_west(tar);
		Player &est = get_east(tar);
		int cnt = 1 + !wst.is_null() + !est.is_null();
		attack(plr,tar,plr.atk * plr.weapon_atk * 1.35l / cnt,atkpos,ddgpos);
		if(!wst.is_null()) attack(plr,wst,plr.atk * plr.weapon_atk * 1.35l / cnt,atkpos,ddgpos);
		if(!est.is_null()) attack(plr,est,plr.atk * plr.weapon_atk * 1.35l / cnt,atkpos,ddgpos);
	}
	else if(plr.weapon_type == M){
		Player &wst = get_west(tar);
		Player &est = get_east(tar);
		attack(plr,tar,plr.atk * plr.weapon_atk * 1.15l,atkpos,ddgpos);
		if(!wst.is_null()) attack(plr,wst,plr.atk * plr.weapon_atk * 0.23l,atkpos,ddgpos);
		if(!est.is_null()) attack(plr,est,plr.atk * plr.weapon_atk * 0.23l,atkpos,ddgpos);
	}
	plr.skill_additional = 1;
}

db weak_skill_val[] = {0,10,12,15,17,20};//weak�������ܵĲ�������ֵ�ǰٷֱ�
db average_skill_val[] = {0,6,7,8,9,10};//average�������ܵĲ�������ֵ�ǰٷֱ�
db strong_skill_val[] = {1,2.1l,2.17l,2.24l,2.32l,2.4l};//strong�������ܵĲ�����ע��0���Ĳ�����1������0
void skill(Player &plr,Player &tar){//������������
	output.output_apply_skill(plr,tar);
	if(plr.type == WEAK){
		output.output_recovery(tar,weak_skill_val[plr.skill_lv] / 100.0l * tar.maxhp);//weak�������ܣ��ö��ѻ�Ѫ
	}
	else if(plr.type == AVERAGE){
		tar.average_skill_effect = average_skill_val[plr.skill_lv];//average�������ܣ��öԷ�ÿ�غϿ�Ѫ
		tar.average_skill_effect_left = 3;//��Ѫdebuff����3���غ�
		//tar.out();
	}
	else if(plr.type == STRONG){
		tar.skill_additional = strong_skill_val[plr.skill_lv];//strong�������ܣ��޸Ķ��ѵļ��ܼӳ�
	}
}

void run(Attack atk){//��ʼһ���غ�
	Player &plr = get_now_player(atk.id);
	//cerr<<"now player: "<<team_name[plr.team]<<" "<<plr.id<<endl;
	get_weak_pskill(plr.team);
	//cerr<<"*****"<<endl;
	get_pskill();
	//cerr<<"*****"<<endl;
	Player &tar = get_target(plr,atk);
	//cerr<<"tar: "<<team_name[tar.team]<<" "<<tar.id<<endl;
	//tar.out();
	assert(!tar.is_died());
	if(atk.type == BASIC) basic_attack(plr,tar,atk.atkpos,atk.ddgpos);
	else if(atk.type == SPECIAL) special_attack(plr,tar,atk.atkpos,atk.ddgpos);
	else skill(plr,tar);
	get_average_skill_effect(!plr.team);
}
int main(){
	srand(time(0));rd();
	//freopen("10.in","r",stdin);
	//freopen("out.out","w",stdout);
	//file("");
	int i,j;
	n[0] = lst[0] = read();n[1] = lst[1] = read();
	for(i = 0;i < 2;++i){
		for(j = 1;j <= n[i];++j) a[i][j].input(i,j);
	}
	t = read();
	for(i = 0;i < t;++i){
		//cout<<"turn "<<i<<":"<<endl;
		Attack atk;
		atk.input(i);
		//cerr<<atk.type<<" "<<atk.target<<" "<<atk.atkpos<<" "<<atk.ddgpos<<"***"<<endl;
		run(atk);
		output.output_all();
	}
	output.end_game();
	//fwrite(bfu,1,bfuu - bfu,stdout);
	//cerr<<clock()<<endl;
	return 0;
}



