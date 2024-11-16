//ALI ZAIN L22-6551
 //Assignment 3
//BSC 3D

#include<iostream>
#include<list>
#include<vector>
#include<fstream>
#include<string>

using namespace std;
template<class T>

class Min_Heap
{
private:
	T* Min_array;
	int capacity;
	int count;
public:
	Min_Heap()
	{
		count = -1;//the index of arr
		Min_array = 0;
		capacity = 0;
	}
	Min_Heap(int size = 0)
	{
		if (size)
		{
			Min_array = new T[size];
			capacity = size;
		}
		count = -1;
	}

	bool IsEmpty()
	{
		if (count == -1)
		{
			return true;
		}
		return false;
	}

	void Insert(T key)
	{
		count++;
		if (count == capacity)  // if the heap is full then double it and copy data
		{
			int temp_cap = capacity * 2;
			T* tempArr = new T[temp_cap];
			for (int i = 0; i < capacity; i++)
			{
				tempArr[i] = Min_array[i];
			}
			capacity = temp_cap;
			delete[]Min_array;
			Min_array = tempArr;
		}
		Min_array[count] = key;
		int i = count;
		//after insertion check the min heap condition parent must be less than childs
		while (i > 0 && Min_array[i] < Min_array[(i - 1) / 2])
		{
			swap(Min_array[i], Min_array[(i - 1) / 2]);
			i = (i - 1) / 2; //i =parent
		}
	}
	void swap(T& a, T& b)
	{
		T t = a;
		a = b;
		b = t;
	}
	void Heapify(int i)
	{
		if (i > count || i < 0)
		{
			return;
		}

		int left = (i * 2) + 1; //left child
		int right = (i * 2) + 2; // right child
		int smallest = i;   // on which heapify is called

		if (left <= count && Min_array[left] < Min_array[smallest])
		{
			smallest = left;
		}

		if (right <= count && Min_array[right] < Min_array[smallest])
		{
			smallest = right;
		}


		if (smallest != i)
		{
			swap(Min_array[i], Min_array[smallest]);
			Heapify(smallest);
		}
	}
	T ExtractMin()
	{
		T val = 0;
		if (count == -1)
		{
			return 0;
		}
		else
		{
			val = Min_array[0];
			Min_array[0] = Min_array[count];
			count--;
			Heapify(0);
		}
		return val;
	}
	~Min_Heap()
	{
		if (Min_array)
		{
			delete[]Min_array;
		}
	}
};




class HospitalData
{

public:
	int id;
	int time;
	HospitalData(int i = 0, int d = 0)
	{
		id = i;
		time = d;
	}
	bool operator<(const HospitalData& h)
	{
		if (time < h.time)
		{
			return true;
		}
		else return false;
	}
	friend ostream& operator<<(ostream& out, const HospitalData h)
	{
		out << "( H" << h.id << "," << h.time << " )";
		return out;
	}
	~HospitalData() {};
};


class RoutingSystem
{
private:
	vector<list<HospitalData>> adj; //adjecncy list
	int size;


public:
	RoutingSystem()
	{
		size = 0;
	}

	// gives optimal route for all
	void FindBestRoute(vector<int>& ti, vector<int>& pr)
	{ //dijiksan algo

		vector<int> time_Taken(size, INT_MAX); // if hospital initialize all the hospitals time with int max
		time_Taken[0] = 0; // if UN so intialize time with 0
		//for track of privious visited hospitals
		vector<int> previous(size, -1);


		Min_Heap<HospitalData> Heap(size);
		HospitalData un(0, 0); //un
		Heap.Insert(un);
		while (!Heap.IsEmpty())
		{
			HospitalData current = Heap.ExtractMin();//cur = un
			for (list<HospitalData>::iterator itr = adj[current.id].begin(); itr != adj[current.id].end(); ++itr)
			{
				HospitalData destination = *itr;
				HospitalData next(destination.id, current.time + destination.time);
				if (next.time < time_Taken[next.id])
				{
					time_Taken[next.id] = next.time;
					previous[next.id] = current.id;
					Heap.Insert(next);
				}
			}
		}
		ti = time_Taken;
		pr = previous;
	}




	void ReadDataFromFile(string filename)
	{
		ifstream fin;
		fin.open(filename);
		if (fin.is_open())
		{
			list<HospitalData> list_of_hospitals;
			//for reading 
			string temp;

			//for storing the time
			string str;

			int id_of_own;
			int id_of_connected;
			int t;
			int index;

			fin >> size; //reads till space
			fin >> temp;
			size++;

			if (temp[0] == 'U')
			{
				fin >> temp;
				while (temp[0] == '(')
				{
					id_of_connected = temp[2] - '0';
					index = 4;
					while (temp[index] != ')')
					{
						str = str + temp[index];
						index++;
					}
					t = stoi(str);
					str.clear();

					HospitalData Hospital(id_of_connected, t);

					list_of_hospitals.push_back(Hospital);
					fin >> temp;
				}
				adj.push_back(list_of_hospitals);
				list_of_hospitals.clear();
				if (temp[0] >= 48 && temp[0] <= 57)//check the ascii of the chartcter matches the int 
				{
					id_of_own = temp[0] - '0'; //to convert str to int
				}
			}

			while (!fin.eof())
			{
				fin >> temp;
				while (temp[0] == '(')
				{
					id_of_connected = temp[2] - '0';
					index = 4;
					while (temp[index] != ')')
					{
						str = str + temp[index];
						index++;
					}

					// to convert string into int
					t = stoi(str);
					str.clear();

					HospitalData HosPital(id_of_connected, t);

					list_of_hospitals.push_back(HosPital);
					temp.clear();
					fin >> temp;
				}
				adj.push_back(list_of_hospitals);
				list_of_hospitals.clear();
			}

		}
		else
		{
			cout << "Error File does not open :( please check the name or .txt extention !\n";
		}
	}



	void Display()
	{
		cout << "\n\nData in file:\n\n";

		for (int i = 0; i < size; i++)
		{
			if (i == 0)
			{
				cout << endl << "UN ---> ";
			}
			else
			{
				cout << endl << "H" << i << " ---> ";
			}
			for (auto itr = adj[i].begin(); itr != adj[i].end(); ++itr)
			{
				cout << *itr << " ";
			}
		}
		cout << endl << endl;
	}


	void Display(vector<int> prev, vector<int> timm)
	{
		cout << "\nBEST ROUTES: \n";
		cout << "--------------------------------------";
		cout << "\n| Hospitals |\tMinimum Time |\tRoute |";
		cout << "\n--------------------------------------";
		int s = 0; //size for each
		vector<int> routes;
		int k;
		for (int i = 0; i < size; i++)
		{
			if (i == 0)
			{
				cout << endl << "UN\t\t0\t\tUN";
			}
			else
			{   //using i for ids and k for lists against i
				k = i;
				cout << endl << endl << "H" << i << "\t\t" << timm[i];
				s = 0;
				routes.clear();
				// check for un j=0
				while (k != 0)
				{
					routes.insert(routes.begin(), k); //insert at start of vector
					s++;
					k = prev[k];
				}
				cout << "\t\tUN";
				for (k = 0; k < s; k++)
				{
					int current_time = timm[routes[k]];
					//previous path time
					if (k > 0)
					{
						current_time = current_time - timm[routes[k - 1]];
					}
					cout << " --- " << current_time << " --> H" << routes[k];
				}
			}
		}
		cout << "\n----------------------------------------------------------------------------\n";
	}

	//void wirteInFile(string fname,int& s)
	//{
	//	
	//	ofstream fout;
	//	fout.open(fname);
	//	
	//	if (fout.is_open())
	//	{
	//		    int choice = 1;
	//			int timeTaken = 0;
	//			int id=0;
	//		
	//		cout << "\n\nNumber of Hospitals: ";
	//		cin >> s;
	//		while (1)
	//		{  
	//			if (s < 1) 
	//			{
	//			cout << "\n Please Enter  positive size: ";
	//			cin >> s;
	//		    }
	//			else
	//			{
	//				break;
	//			}
	//		}
	//		fout << s << "\n"; //to write the size on file
	//		for (int i = 0; i <= s; i++)
	//		{
	//			fout << i << " ";
	//			cout << "\n\nHospitals Directly conected to ";
	//			if (i == 0)
	//			{
	//				cout << "UN : ";
	//			}
	//			else
	//			{
	//				cout << "Hospital " << i;
	//			}
	//			choice = 1;
	//			while (choice == 1)
	//			{
	//				cout << "\n\nEnter ID: ";
	//				cin >> id;
	//				while (1)
	//				{
	//					if (id < 1 || id > s || id == i)
	//					{
	//						cout << endl;
	//						if (id < 1)
	//						{
	//							cout << "Please Enter a positive ID: ";
	//						}
	//						else if (id == i)
	//						{
	//							cout << "same id please Enter new id: ";
	//						}

	//						else if (id > s)
	//						{
	//							cout << "ID greater than no. of hospitals please Enter again: ";
	//						}

	//						cin >> id;

	//					}
	//					else
	//					{
	//						break;
	//					}
	//				}
	//				//writing in the format given
	//				fout << "(H" << id << ",";
	//				cout << "\nEnter the Time Taken to go from ";
	//				if (i == 0)
	//				{
	//					cout << "UN ";
	//				}
	//				else
	//				{
	//					cout << "Hospital " << i<<" ";
	//				}
	//				cout << " to Hospital " << id<<":  ";
	//				cin >> timeTaken;
	//				while (1)
	//				{
	//					if(timeTaken<0)
	//					{
	//						cout << "\nPlease Enter positive Time: ";
	//						cin >> timeTaken;
	//					}
	//					else
	//					{
	//						break;
	//					}
	//				}
	//				fout << timeTaken << ") ";
	//				cout << "\nPress 1 to add another Hospital connected and 2 to move ahead: ";
	//				cin >> choice;
	//				if (choice == 2)
	//				{
	//					break;
	//				}
	//			}
	//			fout << "\n";
	//		}
	//	}
	//	fout.close();

	//}

};







int main()
{
	RoutingSystem RS;
	int c;
	cout << "------------------------- Welcome To ZAYN's Routing System ---------------------------- \n\n\n";
	cout << "Enter 1 for testing prewritten Input file and 2 for inputing the data  file: ";
	cin >> c;

	while (true)
	{
		if (c != 1 && c != 2)
		{
			cout << "Enter Correct choice: ";
			cin >> c;
		}
		else if (c == 1)
		{
			RS.ReadDataFromFile("input.txt");

			RS.Display();
			vector<int>timm;
			vector<int>prevv;
			RS.FindBestRoute(timm, prevv);
			RS.Display(prevv, timm);
			break;
		}
		/*else if (c == 3)
		{

			int size;
			string fileName = "output.txt";
			RS.wirteInFile(fileName, size);
			ReadDataFromFile();
			RS.Display();

			vector<int>timm;
			vector<int>prevv;
			RS.FindBestRoute(timm, prevv);
			RS.Display(prevv,timm);
			cout << endl;
			break;
		}*/
		else if (c == 2)
		{
			string File;
			cout << "Enter the name of file: ";
			cin >> File;
			RS.ReadDataFromFile(File);

			RS.Display();

			vector<int>timm;
			vector<int>prevv;
			RS.FindBestRoute(timm, prevv);
			RS.Display(prevv, timm);

			break;
		}

	}

	cout << endl;
	cout << endl;
	system("pause");
	return 0;
}