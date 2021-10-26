#include <bits/stdc++.h>
using namespace std;

struct link {
    string src, dest;
};
class database {
public:
    set<string> sites;
    map<string,double > score;
    map<string,double > rank;
    map<string, double> CTR;
    map<string, vector<string>> keywords;
    map<string, double> impressions;
    map<string, double> clicks;
    vector<link> links;
    map<string, int> numOfLinksOut;
    map<int, string> results_save;
    database() {
        initialize();
        score_calc();
        start();
    }
    void save() {
        ofstream out("NewImpressions.csv");
        for (const auto i : impressions)
            out << i.first << ',' << i.second << endl;
        out.close();
        remove("Impressions.csv");
        rename("NewImpressions.csv", "Impressions.csv");
        out.open("NewClciks.csv");
        for (const auto j : clicks)
            out << j.first << ',' << j.second << endl;
        out.close();
        remove("Clciks.csv");
        rename("NewClciks.csv", "Clciks.csv");
        out.close();
    }
    void open() {
        int choice;
        cout << "Choose a website by number: ";
        cin >> choice;
            cout << endl;
                cout << "You're now viewing " << results_save[choice] << endl << endl << "Would you like to " << endl << endl << "1. Back to search results" << endl << "2. New Search" << endl << "3. Exit" << endl << endl << "Type in your choice: ";
                clicks[results_save[choice]]++;
            cin >> choice;
            cout << endl;
            if (choice == 1)
            {
                for (const auto i : results_save)
                    cout << i.first << ". " << i.second << endl << endl;
                open();
            }
            else if (choice == 2)
                search_request();
            else if (choice == 3)
            {
                save();
                return;
            }
    }
    void search(char KWords[]) {
        
        set<pair<double, string>> results;
        char* ptr_1, * ptr_2, * ptr_3, * ptr_4;
        string searched(KWords);
        vector<string> search_words;
        int order = 1;
        bool found = false;
        if (searched.find(" OR ") != string::npos)
        {
            char* token = strtok_s(KWords, " OR ", &ptr_1);
                search_words.push_back(token);
                token = strtok_s(NULL, " OR ", &ptr_1);
                search_words.push_back(token);
            for ( auto i : search_words)
                for ( auto j : keywords)
                    if (find(j.second.begin(), j.second.end(), i) != j.second.end())
                    {
                        results.insert(make_pair(score[j.first],j.first ));
                        impressions[j.first]++;
                    }
        }
        else if (searched.find("AND") != string::npos)
        {
            char* token = strtok_s(KWords, " AND ", &ptr_2);
                search_words.push_back(token);
                token = strtok_s(NULL, " AND ", &ptr_2);
            search_words.push_back(token);
            for (const auto i : keywords)
            {
                int found = 0;
                for (const auto j : search_words)
                    if (find(i.second.begin(), i.second.end(), j) != i.second.end())
                        found+=1;
                if (found == search_words.size() )
                {
                    results.insert(make_pair(score[i.first], i.first));
                    impressions[i.first]++;
                }
            }
        }
        else if (searched.find(" ") != string::npos && searched.find('/"') == string::npos)
        {
            char* token = strtok_s(KWords, " ", &ptr_3);
            if (token != NULL)
            {
                search_words.push_back(token);
                token = strtok_s(NULL, " ", &ptr_3);
            }
            for (const auto i : search_words)
                for (const auto j : keywords)
                    if (find(j.second.begin(), j.second.end(), i) != j.second.end() )
                    {
                        results.insert(make_pair(score[j.first], j.first));
                        
                        impressions[j.first]++;
                    }
        }
        else if (searched.find("\"") != string::npos)
        {
            char* token = strtok_s(KWords, "\"", &ptr_4);
                search_words.push_back(token);
                token = strtok_s(NULL, "\"", &ptr_4);
            for (const auto i : search_words)
                for (const auto j : keywords)
                    if (find(j.second.begin(), j.second.end(), i) != j.second.end() )
                    {
                        results.insert(make_pair(score[j.first], j.first));
                        impressions[j.first]++;
                    }
        }
        else
        {
            search_words.push_back(searched);
            for (const auto i : search_words)
                for (const auto j : keywords)
                    if (find(j.second.begin(), j.second.end(), i) != j.second.end() )
                    {
                        results.insert(make_pair(score[j.first], j.first));
                        impressions[j.first]++;
                    }
        }
        int count = 1;
        for (auto i : results)
        {
            results_save.insert(make_pair(count, i.second));
            count++;
        }
        cout << "Search results: " << endl << endl;
        for (const auto i : results) 
            cout <<  i.second << endl;
        cout << endl;
    }
    void search_request() {
            results_save.clear();
            cout << "Search the web ";
            string input;
            int choice;
            cin.ignore();
            getline(cin, input);
            char* char_arr;
            char_arr = &input[0];
            cout << endl << endl;
            search(char_arr);
            cout << endl << "Enter your choice : What do you need now? \n(1)Open a web page\n(2)Perform a new search query \n(3) Exit program";
            cin >> choice;
            cout << endl;
            if (choice == 1)
                open();
            else if (choice == 2)
                search_request();
            else 
            {
                save();
                return;
            }
        }

    void start() {
        cout << "Welcome!\n\nEnter your choice : What do you need now? \n(1)Perform search query\n(2)Exit program";
        int choice;
        cin >> choice;
        cout << endl;
        if (choice == 1)
            search_request();
        else
        {
            //save();
            return;
        }

    }
    void score_calc() {
        for (const auto j : sites) {
            CTR[j] = clicks[j] / impressions[j];
        }
        for (const auto i : sites)
            score[i] = (0.4 * rank[i]) + ((((1 - (0.1 * impressions[i] / (1 + (0.1 * impressions[i])))) * rank[i]) + ((0.1 * impressions[i] / (1 + (0.1 * impressions[i]))) * CTR[i])) * 0.6);
    }
    void rank_calc(){
        map<string, double > tempo_rank=rank;
        double lowest = 99;
        double highest = -1;
            for (auto i : sites) {
                double temp_rank = 0;
                for (auto j : links) {
                    if (j.dest == i)
                        temp_rank += (tempo_rank[j.src]) / numOfLinksOut[j.src];
                }
                rank[i] = temp_rank;
                if (rank[i] < lowest)
                    lowest = rank[i];
                if (rank[i] > highest)
                    highest = rank[i];
            }
            double diff = highest - lowest;
            for (auto q : sites)
                rank[q] = (rank[q] - lowest) / diff;
    }
    void initialize() {
        ifstream in;
        string temp, word;
        vector <string> tempo;
        link temp_link;
        in.open("WebGraph.csv");
        while (in >> temp) {
            //cout << temp << endl;
            stringstream s(temp);
            while (getline(s, word, ','))
                tempo.push_back(word);
            temp_link.src = tempo[0];
            temp_link.dest = tempo[1];
            links.push_back(temp_link);
            tempo.clear();
        }
        for ( auto i : links)
        {
            sites.insert(i.src);
            sites.insert(i.dest);
        }
        for (auto i : sites) {
            numOfLinksOut.insert(make_pair(i, 0));
        }
        in.close();
        set<string>::iterator itr;
        for (itr = sites.begin(); itr != sites.end(); itr++)
            rank.insert(make_pair( *itr ,1.0 / sites.size()  ));
        for (auto i : links) {
            numOfLinksOut[i.src]++;
        }   
        rank_calc();
        string temp2, word2,site;
        vector <string> tempo2;
        in.open("KeyWords.csv");
        while (in >> temp2) {
           // cout << temp2 << endl;
            stringstream s2(temp2);
            getline(s2, word2, ',');
            site = word2;
            while (getline(s2, word2, ',')) {
                if (word2.find('+') != string::npos) {
                    word2.replace(word2.find('+'), 1, " ");
                }
                tempo2.push_back(word2);
            }
            keywords.insert(make_pair(site, tempo2));
            tempo2.clear();
        }
        in.close();
        string temp3, word3, site2;
        vector <int> tempo3;
        in.open("Impressions.csv");
        while (in >> temp3) {
            //cout << temp3 << endl;
            stringstream s3(temp3);
            getline(s3, word3, ',');
            site2 = word3;
            while (getline(s3, word3, ',')) {
                tempo3.push_back(stoi(word3));
            }
            impressions.insert(make_pair(site2, tempo3[0]));
            tempo.clear();
        }
        in.close();
        string temp4, word4, site3;
        vector <int> tempo4;
        in.open("Clicks.csv");
        while (in >> temp4) {
            //cout << temp4 << endl;
            stringstream s4(temp4);
            getline(s4, word4, ',');
            site3 = word4;
            while (getline(s4, word4, ',')) {
                tempo4.push_back(stoi(word4));
            }
            clicks.insert(make_pair(site3, tempo4[0])); 
            tempo.clear();
        }
        in.close();
    }
};
int main()
{
    database x;
}