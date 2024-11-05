#include <iostream>
#include <curl/curl.h>
#include <tinyxml2.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
using namespace std;
string repo_url, name_pack, graph_depth, graph_path, mer_path,url;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//загрузка содержимого файла по ссылке
string downloadFile(string url) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

struct Dependency {
    string groupId;
    string artifactId;
    string version;
};

//поиск зависимостей в xml файле
vector<Dependency> parseXML(string xmlContent) {
    vector<Dependency> dependencies;
    size_t pos = 0;
    size_t endPos = 0;
    string tag="";
    while ((pos = xmlContent.find('<', pos)) != string::npos) {
        endPos = xmlContent.find('>', pos);
        if (endPos == string::npos) break;

        tag = xmlContent.substr(pos + 1, endPos - pos - 1);
        if (tag.find("dependencies")!=string::npos)
            break;
        pos = endPos + 1;
    }
    tag="";
    
    while (true){
        pos = xmlContent.find('<', pos);
        endPos = xmlContent.find('>', pos);
        tag = xmlContent.substr(pos + 1, endPos - pos - 1);
        if (tag=="/dependencies")
            break;
        if (tag=="dependency"){
            Dependency dependency;
            
            pos = xmlContent.find("<groupId>", pos);
            endPos = xmlContent.find("</groupId>", pos);
            tag = xmlContent.substr(pos + 9, endPos - pos - 9);
            dependency.groupId = tag;
            
            pos = xmlContent.find("<artifactId>", pos);
            endPos = xmlContent.find("</artifactId>", pos);
            tag = xmlContent.substr(pos + 12, endPos - pos - 12);
            dependency.artifactId = tag;
                
            int pred_pos = pos;
            pos = xmlContent.find("<version>", pos);
            if (pos<endPos+25 && pos>pred_pos){
                endPos = xmlContent.find("</version>", pos);
                tag = xmlContent.substr(pos + 9, endPos - pos - 9);
                dependency.version = tag;
            }
            else endPos=pred_pos;

            dependencies.push_back(dependency);
        }
        pos = endPos + 1;
    }
    return dependencies;
}

//поиск версии в файле(когда записана переменная)
string findReal(string str,string tek_xml){
    string s = str+">";
    int pos = tek_xml.find("<"+s, 0);
    int endPos = tek_xml.find("</"+s, pos);
    string tag = tek_xml.substr(pos + 2 +str.size(), endPos - pos - 2 - str.size());
    return tag;
}

//формирование ссылки через ссылку на репозиторий и название пакета
string constructPomUrl(Dependency dep,string pred_xml) {
    string baseUrl = "https://repo.maven.apache.org/maven2/";
    string groupPath = dep.groupId;
    for (int i = 0;i<groupPath.size();i++)
        if (groupPath[i]=='.')
            groupPath[i]='/';
    if (dep.artifactId[0]=='$')
        dep.artifactId = findReal(dep.artifactId,pred_xml);
    if (dep.version[0]=='$')
        dep.version = findReal(dep.version.substr(2,dep.version.size()-3),pred_xml);
    
    return baseUrl + groupPath + "/" + dep.artifactId + "/" + dep.version + "/" + dep.artifactId + "-" + dep.version + ".pom";
}

//построение графа
void buildDependencyGraph(Dependency rootDep, int depth, unordered_map<string, pair<vector<Dependency>,int>>& graph,string pred) {
    if (depth <= 0) return;
    
    string pomUrl;
    if (depth!=stoi(graph_depth))
        pomUrl = constructPomUrl(rootDep,pred);
    else pomUrl = url;
    //cout<<"url: "<<pomUrl<<endl;
    string xmlContent = downloadFile(pomUrl);
    //cout<<"downloaded.\n";
    vector<Dependency> dependencies = parseXML(xmlContent);
    //cout<<"parsed.\n";
    
    string depKey = rootDep.groupId + ":" + rootDep.artifactId + ":" + rootDep.version;
    graph[depKey] = make_pair(dependencies,depth);

    for (const auto& dep : dependencies) {
        
        string depKey = dep.groupId + ":" + dep.artifactId + ":" + dep.version;
        if (dep.version!="" && graph.find(depKey) == graph.end())
            buildDependencyGraph(dep, depth - 1, graph,xmlContent);
        
    }
}

//формирование mermaid кода для графа
string generateMermaidCode(unordered_map<string, pair<vector<Dependency>,int>>& graph){
    if (graph.size()==0)
        return "The graph is empty!";
    string mermaid = "graph TD\n";
    for (auto versh:graph)
        for (auto c:versh.second.first)
            mermaid+="    " + versh.first + " --> " + c.groupId + ":" + c.artifactId + ":" + c.version + "\n";
    return mermaid;
}

// сохранение mermaid кода в файл mmd
void saveToFile(string content, string filename) {
    ofstream file(filename);
    if (file.is_open())
        file << content;
    else cout<<"Error!\n";
    file.close();
}

//формирование png из mermaid кода
void generatePngFromMermaid(string inputFile, string outputFile) {
    string command = "mmdc -i " + inputFile + " -o " + outputFile;
    int result = system(command.c_str());
    if (result != 0) {
        cout << "Failed to generate PNG from Mermaid" << endl;
    }
    else cout<<"png generated\n";
}

//чтение ключей
void initialization(string mer_p,string name_p, string graph_p, string graph_d, string repo_u){
    mer_path = mer_p;
    name_pack = name_p;
    graph_path = graph_p;
    graph_depth = graph_d;
    repo_url = repo_u;
}

#ifndef UNIT_TEST
int main(int argc,char* argv[]) {
    mer_path = argv[1];
    name_pack = argv[2];
    graph_path = argv[3];
    graph_depth = argv[4];
    repo_url = argv[5];

    initialization(argv[1],argv[2],argv[3],argv[4],argv[5]);

    cout<<"path to mermaid program: "<<mer_path<<endl;
    cout<<"name of packet: "<<name_pack<<endl;
    cout<<"path to dependencies graph: "<<graph_path<<endl;
    cout<<"depth of graph: "<<graph_depth<<endl;
    cout<<"url of git repo: "<<repo_url<<endl;
    
    //https://repo.maven.apache.org/maven2/
    //org.apache.maven:maven-parent:43

    int pos1 = name_pack.find(':',0);
    int endPos1 = name_pack.find(':',pos1+1);
    string group = name_pack.substr(0,pos1-1);
    string artifact = name_pack.substr(pos1+1,endPos1-pos1-1);
    string version = name_pack.substr(endPos1+1,name_pack.size()-endPos1-1);
    
    int i = 0;
    while (name_pack[i]!=':'){
        if (name_pack[i]=='.')
            name_pack[i]='/';
        i++;
    }
    
    while (i<name_pack.size()){
        if (name_pack[i]==':')
            name_pack[i]='/';
        i++;
    }
    name_pack+='/'+artifact+'-'+version+".pom";

    url = repo_url+name_pack;
    //cout<<"url: "<<url<<endl;
    string pomXmlContent = downloadFile(url);
    
    Dependency rootDep;
    /*
    int pos = pomXmlContent.find("<parent>", pos);
    int endPos = pomXmlContent.find("</parent>", pos);
    string tag = pomXmlContent.substr(pos + 1, endPos - pos - 1);
    
    pos = pomXmlContent.find("<groupId>", pos);
    endPos = pomXmlContent.find("</groupId>", pos);
    tag = pomXmlContent.substr(pos + 9, endPos - pos - 9);
    rootDep.groupId = tag;
    
    pos = pomXmlContent.find("<artifactId>", pos);
    endPos = pomXmlContent.find("</artifactId>", pos);
    tag = pomXmlContent.substr(pos + 12, endPos - pos - 12);
    rootDep.artifactId = tag;
    
    pos = pomXmlContent.find("<version>", pos);
    endPos = pomXmlContent.find("</version>", pos);
    tag = pomXmlContent.substr(pos + 9, endPos - pos - 9);
    rootDep.version = tag;
    */
    rootDep.groupId = group;
    rootDep.artifactId = artifact;
    rootDep.version = version;

    cout<<rootDep.groupId+":"+rootDep.artifactId+":"+rootDep.version<<endl;    
    
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, stoi(graph_depth), dependencyGraph,pomXmlContent);
    
    /*
    for (const auto& node : dependencyGraph) {
        cout << "Package: " << node.first << " "<<node.second.second<<endl;
        for (const auto& dep : node.second.first) {
            cout << "  Dependency: " << dep.groupId << ":" << dep.artifactId << ":" << dep.version << endl;
        }
    }
    */

    string mermaid_code = generateMermaidCode(dependencyGraph);
    cout<<mermaid_code<<endl;
    
    string inputFile = mer_path+"graph.mmd";
    string outputFile = graph_path+"graph.png";
    saveToFile(mermaid_code, inputFile);
    generatePngFromMermaid(inputFile, outputFile);
    return 0;
}
#endif