#include <iostream>
#include <curl/curl.h>
#include <tinyxml2.h>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
string git_url, name_pack, graph_depth, graph_path, mer_path;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string downloadFile(string url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

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
    std::string groupId;
    std::string artifactId;
    std::string version;
};

vector<Dependency> parseXML(string xmlContent) {
    vector<Dependency> dependencies;
    size_t pos = 0;
    size_t endPos = 0;
    string tag="";
    while ((pos = xmlContent.find('<', pos)) != std::string::npos) {
        endPos = xmlContent.find('>', pos);
        if (endPos == std::string::npos) break;

        tag = xmlContent.substr(pos + 1, endPos - pos - 1);
        if (tag.find("dependencies")!=string::npos)
            break;
        pos = endPos + 1;
    }
    tag="";
    //cout<<"found depend\n";
    while (true){
        pos = xmlContent.find('<', pos);
        endPos = xmlContent.find('>', pos);
        tag = xmlContent.substr(pos + 1, endPos - pos - 1);
        //cout<<tag<<endl;
        if (tag=="/dependencies")
            break;
        if (tag=="dependency"){
            Dependency dependency;
            
            pos = xmlContent.find("<groupId>", pos);
            endPos = xmlContent.find("</groupId>", pos);
            tag = xmlContent.substr(pos + 9, endPos - pos - 9);
            dependency.groupId = tag;
            //cout<<"group: "<<tag<<endl;

            pos = xmlContent.find("<artifactId>", pos);
            endPos = xmlContent.find("</artifactId>", pos);
            tag = xmlContent.substr(pos + 12, endPos - pos - 12);
            dependency.artifactId = tag;
            //cout<<"artifact: "<<tag<<endl;
                
            int pred_pos = pos;
            pos = xmlContent.find("<version>", pos);
            if (pos<endPos+25 && pos>pred_pos){
                endPos = xmlContent.find("</version>", pos);
                tag = xmlContent.substr(pos + 9, endPos - pos - 9);
                //cout<<"version: "<<tag<<endl;
                dependency.version = tag;
            }
            else {
                //cout<<"gabella "<<pos<<" "<<endPos<<endl;
                endPos=pred_pos;
            }
            //cout<<"version: "<<tag<<endl;
            
            dependencies.push_back(dependency);
        }
        pos = endPos + 1;
    }
    //cout<<"all\n";
    return dependencies;
}

string findReal(string str,string tek_xml){
    string s = str+">";
    int pos = tek_xml.find("<"+s, 0);
    int endPos = tek_xml.find("</"+s, pos);
    string tag = tek_xml.substr(pos + 2 +str.size(), endPos - pos - 2 - str.size());
    return tag;
}

string constructPomUrl(Dependency dep,string pred_xml) {
    string baseUrl = "https://repo.maven.apache.org/maven2/";
    string groupPath = dep.groupId;
    for (int i = 0;i<groupPath.size();i++)
        if (groupPath[i]=='.')
            groupPath[i]='/';
    if (dep.artifactId[0]=='$')
        dep.artifactId = findReal(dep.artifactId,pred_xml);
    if (dep.version[0]=='$'){
        //cout<<"gabella\n";
        dep.version = findReal(dep.version.substr(2,dep.version.size()-3),pred_xml);
        //cout<<dep.version<<endl;
    }
    //std::replace(groupPath.begin(), groupPath.end(), '.', '/');  // Заменяем точки на слеши

    return baseUrl + groupPath + "/" + dep.artifactId + "/" + dep.version + "/" + dep.artifactId + "-" + dep.version + ".pom";
}

void buildDependencyGraph(Dependency rootDep, int depth, std::unordered_map<std::string, pair<vector<Dependency>,int>>& graph,string pred) {
    if (depth == 0) return;
    
    string pomUrl = constructPomUrl(rootDep,pred);
    //cout<<"url: "<<pomUrl<<endl;
    string xmlContent = downloadFile(pomUrl);
    //cout<<"downloaded.\n";
    vector<Dependency> dependencies = parseXML(xmlContent);
    //cout<<"parsed.\n";
    
    std::string depKey = rootDep.groupId + ":" + rootDep.artifactId + ":" + rootDep.version;
    graph[depKey] = make_pair(dependencies,depth);

    for (const auto& dep : dependencies) {
        
        std::string depKey = dep.groupId + ":" + dep.artifactId + ":" + dep.version;
        if (dep.version!="" && graph.find(depKey) == graph.end()) {
            //graph[depKey] = dependencies;  // Добавляем зависимости в граф
            // Для каждой зависимости рекурсивно строим граф, уменьшая глубину
            buildDependencyGraph(dep, depth - 1, graph,xmlContent);
        }
    }
}

int main(int argc,char* argv[]) {
    mer_path = argv[1];
    name_pack = argv[2];
    graph_path = argv[3];
    graph_depth = argv[4];
    git_url = argv[5];
    cout<<"path of mermaid program: "<<mer_path<<endl;
    cout<<"name of packet: "<<name_pack<<endl;
    cout<<"path of dependencies graph: "<<graph_path<<endl;
    cout<<"depth of graph: "<<graph_depth<<endl;
    cout<<"url of git repo: "<<git_url<<endl;
    
    std::string url = "https://raw.githubusercontent.com/apache/maven/master/pom.xml";
    std::string pomXmlContent = downloadFile(url);
    //cout<<pomXmlContent<<endl;
    
    Dependency rootDep;
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
        
    std::unordered_map<std::string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, stoi(graph_depth), dependencyGraph,pomXmlContent);

    /*
    if (!pomXmlContent.empty())
        parseXML(pomXmlContent);
    */

    
    for (const auto& node : dependencyGraph) {
        std::cout << "Package: " << node.first << " "<<node.second.second<<endl;
        for (const auto& dep : node.second.first) {
            std::cout << "  Dependency: " << dep.groupId << ":" << dep.artifactId << ":" << dep.version << std::endl;
        }
    }
    
    return 0;
}
