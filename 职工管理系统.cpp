#include<iostream>
#include<vector>
#include<memory>
#include<algorithm>
using namespace std;

class yuan {
public:
	yuan(int y,string bh,string n):y_id(y),bh(bh),name(n){}
	int y_id;
	string bh;
	string name;
	virtual string work() = 0;
	virtual string gw() = 0;
	virtual ~yuan(){}
};
class boss:public yuan {
public:
	boss(int y, string bh, string n) :yuan(y, bh, n){}
	string work() {
		return "管理员工";
	}
	string gw() {
		return "老板";
	}
};
class pao:public yuan {
public:
	pao(int y, string bh, string n) :yuan(y, bh, n) {}
	string work() {
		return "干活";
	}
	string gw() {
		return "员工";
	}
};
static ostream& operator<<(ostream& cout, yuan& p) {
	cout << "员工编号：" << p.y_id << " 员工名字：" << p.name << " 部门编号：" << p.bh << " 岗位：" << p.gw() << " 工作任务：" << p.work() << endl;
	return cout;
}
static void cai() {
	cout << "0.退出管理程序" << endl;
	cout << "1.增加职工信息" << endl;
	cout << "2.显示职工信息" << endl;
	cout << "3.删除职工信息" << endl;
	cout << "4.修改职工信息" << endl;
	cout << "5.查找职工信息" << endl;
	cout << "6.按照编号排序" << endl;
	cout << "7.清空所有文档" << endl;
}
static bool px(unique_ptr<yuan> &a,unique_ptr<yuan> &b) {
	return a->y_id < b->y_id;
}
int main() {
	using v = vector<unique_ptr<yuan>>;
	v arr;
	int sr = 0;
	int id = 0, w, sz = 0, z = 0;;
	string name, bh,sc;
	while (1) {
		system("pause");
		system("cls");
		cai();
		cout << "请选择：";
		cin >> sr;
		switch (sr) {
		case 0:
			cout << "欢迎下次使用！";
			return 0;
			break;
		case 1: {
			cout << "请输入增加员工数量：";
			cin >> sz;
			for (int i = 1; i <= sz; i++) {
				cout << "请输入第" << i << "位员工信息" << endl;
				cout << "请输入员工编号：";
				cin >> id;
				cout << "请输入名字：";
				cin >> name;
				cout << "请输入部门编号：";
				cin >> bh;
				cout << "请输入岗位：（1 老板 2 员工）";
				cin >> w;
				if (w == 1) {
					arr.push_back(make_unique<boss>(id, bh, name));
				}
				else if (w == 2)arr.push_back(make_unique<pao>(id, bh, name));
			}
			cout << "输入完成" << endl;
			cout << endl;
			break;
		}
		case 2:
			if (arr.empty())cout << "当前没有员工" << endl;
			else {
				for (const auto& i : arr) {
					cout << *i << endl;
				}
			}
			break;
		case 3:
			if (arr.empty())cout << "当前没有员工" << endl;
			else {
				cout << "请输入要删除的员工:";
				cin >> sc;
				for (int i = 0; i < arr.size(); i++) {
					if (sc == arr[i]->name) {
						arr.erase(arr.begin() + i);
						cout << "删除成功" << endl;
						z = 1;
						break;
					}
					else z = 0;
				}
				if (!z) cout << "没找到" << endl; cout << endl;
			}
			break;
		case 4:
			if (arr.empty())cout << "当前没有员工" << endl;
			else {
				cout << "请输入要修改的员工:";
				cin >> sc;
				for (int i = 0; i < arr.size(); i++) {
					if (sc == arr[i]->name) {
						cout << "请输入修改后的员工编号：";
						cin >> id;
						cout << "请输入修改后的名字：";
						cin >> name;
						cout << "请输入修改后的部门编号：";
						cin >> bh;
						cout << "请输入修改后的岗位：（1 老板 2 员工）";
						cin >> w;
						if (w == 1) {
							arr[i] = (make_unique<boss>(id, bh, name));
						}
						else if (w == 2)arr[i] = (make_unique<pao>(id, bh, name));
						z = 1;
						break;
					}
					else z = 0;
				}
				if (!z) cout << "没找到" << endl; cout << endl;
			}
			break;
		case 5:
			if (arr.empty())cout << "当前没有员工" << endl;
			else{
			cout << "请输入要查找的员工:";
			cin >> sc;
			for (int i = 0; i < arr.size(); i++) {
				if (sc == arr[i]->name) {
					cout << "找到了" << endl;
					cout << *arr[i] << endl;
					z = 1;
					break;
				}
				else z = 0;
			}
			if (!z) cout << "没找到" << endl; cout << endl;
			}
			break;
		case 6:
			if (arr.empty())
				cout << "当前没有员工" << endl;
			else {
				sort(arr.begin(), arr.end(), px);
				cout << "排序完成" << endl;
			}
			break;
		case 7:
			arr.clear();
			cout << "清空完成" << endl;
			break;
		}
	}
	return 0;
}