#define UNIT_TEST // предотвращает компиляцию main() в main.cpp
#include <iostream>
#include "main.cpp"
#include <gtest/gtest.h>
#include <curl/curl.h>
#include <tinyxml2.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

//глубина графа меньше либо равно ноля
TEST(AddTest, HandlesPositiveInput_1) {
    initialization("../mermaid/", "org.apache.maven:maven-parent:43" ,"../", "-1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.maven";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, -1, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    EXPECT_EQ(mermaid_code,"The graph is empty!");
}

//проверка содержимого xml файла
TEST(AddTest, HandlesPositiveInput_2) {
    initialization("../mermaid/", "org.codehaus.plexus:plexus-utils:4.0.1" ,"../", "1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/codehaus/plexus/plexus-utils/4.0.1/plexus-utils-4.0.1.pom";
    string pomXmlContent = downloadFile(url);
    int pos = pomXmlContent.find("<parent>", pos);
    int endPos = pomXmlContent.find("</parent>", pos);
    string tag = pomXmlContent.substr(pos + 1, endPos - pos - 1);
    string example = "parent>\n    <groupId>org.codehaus.plexus</groupId>\n    <artifactId>plexus</artifactId>\n    <version>17</version>\n  ";
    EXPECT_EQ(tag,example);
}


//проверка mermaid кода для глубины 1
TEST(AddTest, HandlesPositiveInput_3) {
    initialization("../mermaid/", "org.apache.maven:maven-parent:43" ,"../", "1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.maven";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, 1, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    string example = "";
    example = "graph TD\n    org.apache.maven:maven-parent:43 --> org.eclipse.sisu.inject\n    org.apache.maven:maven-parent:43 --> org.eclipse.sisu.plexus\n    org.apache.maven:maven-parent:43 --> plexus-utils\n    org.apache.maven:maven-parent:43 --> plexus-xml\n    org.apache.maven:maven-parent:43 --> junit-bom\n";
    EXPECT_EQ(mermaid_code,example);
}

//проверка mermaid кода для глубины 2
TEST(AddTest, HandlesPositiveInput_4) {
    initialization("../mermaid/", "org.apache.maven:maven-parent:43" ,"../", "2", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.maven";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, 2, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    string example = "graph TD\n    org.junit:junit-bom:${versions.junit5} --> junit-jupiter\n    org.junit:junit-bom:${versions.junit5} --> junit-jupiter-api\n    org.junit:junit-bom:${versions.junit5} --> junit-jupiter-engine\n    org.junit:junit-bom:${versions.junit5} --> junit-jupiter-migrationsupport\n    org.junit:junit-bom:${versions.junit5} --> junit-jupiter-params\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-commons\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-console\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-engine\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-jfr\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-launcher\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-reporting\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-runner\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-suite\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-suite-api\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-suite-commons\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-suite-engine\n    org.junit:junit-bom:${versions.junit5} --> junit-platform-testkit\n    org.junit:junit-bom:${versions.junit5} --> junit-vintage-engine\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> jmh-core\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> jmh-generator-annprocess\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> junit-jupiter\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> plexus-utils\n    org.codehaus.plexus:plexus-utils:${version.plexus-utils} --> plexus-xml\n    org.codehaus.plexus:plexus-utils:${version.plexus-utils} --> junit-jupiter-api\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> guice\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> javax.annotation-api\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> cdi-api\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.eclipse.sisu.inject\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> plexus-component-annotations\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> plexus-classworlds\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> plexus-utils\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> plexus-xml\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> slf4j-api\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> osgi.core\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> junit\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> logback-classic\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> guice\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> slf4j-api\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> osgi.core\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> javax.annotation-api\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> cdi-api\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> servlet-api\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> guice-servlet\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> junit\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> junit-jupiter-api\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> testng\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> logback-classic\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> guice-assistedinject\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.apache.felix.framework\n    org.apache.maven:maven-parent:43 --> org.eclipse.sisu.inject\n    org.apache.maven:maven-parent:43 --> org.eclipse.sisu.plexus\n    org.apache.maven:maven-parent:43 --> plexus-utils\n    org.apache.maven:maven-parent:43 --> plexus-xml\n    org.apache.maven:maven-parent:43 --> junit-bom\n";
    EXPECT_EQ(mermaid_code,example);
}
