#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

bool choseYesOrNot()        //Функция ввода ответа пользователя
{
    string chose;
    cin >> chose;
    while ((chose != "Да") && (chose != "да") && (chose != "Нет") && (chose != "нет"))
        {
            cout << "Ошибка. Ответом может быть 'Да' или 'Нет'" << endl;
            cin >> chose;
        }

    if ((chose == "да") || (chose == "Да"))
    {
        return true;
    }
    else
    {
        return false;
    }    
}

int NumberOfFiles(string placeLink)         //Функция подсчета количества файлов     
{
    ifstream filemess;

    bool FIF = true;
    int s = 50;
    int g = 0;

    while (FIF)
    {
        filemess.open(placeLink + "\\messages"  + to_string(s) + ".html", ios_base::in);
        if (filemess.is_open())
        {            
            g++;
            //cout << s << endl;
            s += 50;
            filemess.close();
        }
        else
        {           
            FIF = false;
        }        
    }
    filemess.close();
    return g;
}

int SearchImage(int s, bool OpenLinksBool, string placeLink, int choseObject)        //Функция поиска изображений в файле
{
    ifstream filemess;
    
    string link;
    char stopkey = {}, line[30000];
    string str, stopword = "</html>";
    int currentpos = 0, i = 0;
    size_t pos = 0;
    bool person = true;
    LPCWSTR wideString;
    LPCTSTR wideStringFile;
    wstring temp;
    LPCWSTR open;

    filemess.open(placeLink + "\\messages" + to_string(s) + ".html", ios_base::in);
    filemess.getline(line, 30000);
        
    str = string(line);   
        
    while (str != stopword)     //Проход по файлу 
    {               
        filemess.getline(line, 30000);
        str = string(line);
        link = "";

        if (choseObject > 0)
        {
            if (choseObject == 1)
            {
                if (str.find("Вы") != string::npos)
                {
                    person = true;
                }
                if (str.find("https://vk.com/id") != string::npos)
                {
                    person = false;
                }
            }
            else
            {
                if (str.find("Вы") != string::npos)
                {
                    person = false;
                }
                if (str.find("https://vk.com/id") != string::npos)
                {
                    person = true;
                }
            }
            
        }     

        if (person == true)
        {
            pos = str.find("https://sun");
        }

        if ((pos != string::npos) && (person == true))       //Работа с ссылкой в строке
        {
            i++;
                                                                     
            if (OpenLinksBool == true)
            {
                stopkey = line[pos];
                currentpos = pos;

                while (stopkey != '>')     //Вырез строки
                {
                    link.append(str, currentpos, 1);
                    currentpos++;
                    stopkey = line[currentpos];

                }

                temp = wstring(link.begin(), link.end());
                wideString = temp.c_str();
                open = L"open";
                             
                ShellExecute(NULL, open, wideString, 0, 0, SW_SHOWNORMAL);
            }                                   
        }                
    }
    filemess.close();
    return i;
}

int main()
{    
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);

    int minf = 0,maxf = 0, i = 0, choseObject = 0;
    string OpenLinks, placeLink,ignorePers, targetPers, chose;
    bool OpenLinksBool = false, RestartChoseBool = true;    
    
    cout << "Введите расположение папки с файлами" << endl;
    cin >> placeLink;
        
    while (RestartChoseBool == true)
    {
        i = 0;
        choseObject = 0;

        cout << "С какого номера файла начать поиск(min 50)?" << endl;
        cin >> minf;
        while ((minf % 50 != 0) || (minf < 0))
        {
            cout << "Ошибка. Номер должен быть больше 0 и кратен 50" << endl;
            cin >> minf;
        }

        cout << "До какого файла провести поиск(max " << NumberOfFiles(placeLink) * 50 << ") ? " << endl;
        cin >> maxf;
        while ((maxf % 50 != 0) || (maxf < 0))
        {
            cout << "Ошибка. Номер должен быть больше 0 и кратен 50" << endl;
            cin >> maxf;
        }

        cout<< "Выбрать объект поиска(Только изображения объекта будут извлечены)?" << endl;        
        if (choseYesOrNot())
        {
            cout << "Выбрать получателя введите \"1\". Выбрать отправителя введите \"2\"" << endl;
            cin >> choseObject;
            while ((choseObject != 1) && (choseObject != 2))
            {
                cout << "Ошибка. Ответом может быть \"1\"или \"2\"" << endl;
                cin >> choseObject;
            }
        }

        cout << "Открыть ссылки по завершению программы?" << endl;
        OpenLinksBool = choseYesOrNot();
       
        for (int s = minf; s <= maxf; s += 50)
        {
            i += SearchImage(s, OpenLinksBool, placeLink, choseObject);
            cout << s << endl;
        }

        cout << "Найдено ссылок = " << i;
        cout << endl;
        cout << "Запустить программу заново?" << endl;              
        RestartChoseBool = choseYesOrNot();                
    }        
    cout << "Программа завершена" << std::endl;
    return 0;
}