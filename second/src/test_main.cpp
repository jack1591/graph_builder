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

TEST(AddTest, HandlesPositiveInput_1) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.apache.maven:maven-parent:43" ,"C:/Users/jackt/conf_upr/second/", "-1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.mave";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, -1, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    cout<<mermaid_code<<endl;
    EXPECT_EQ(mermaid_code,"The graph is empty!");
}

TEST(AddTest, HandlesPositiveInput_2) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.codehaus.plexus:plexus-utils:4.0.1" ,"C:/Users/jackt/conf_upr/second/", "1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/codehaus/plexus/plexus-utils/4.0.1/plexus-utils-4.0.1.pom";
    string pomXmlContent = downloadFile(url);
    int pos = pomXmlContent.find("<parent>", pos);
    int endPos = pomXmlContent.find("</parent>", pos);
    string tag = pomXmlContent.substr(pos + 1, endPos - pos - 1);
    string example = "parent>\n    <groupId>org.codehaus.plexus</groupId>\n    <artifactId>plexus</artifactId>\n    <version>17</version>\n  ";
    cout<<tag<<endl;
    EXPECT_EQ(tag,example);
}

TEST(AddTest, HandlesPositiveInput_3) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.apache.maven:maven-parent:43" ,"C:/Users/jackt/conf_upr/second/", "1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.mave";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, 1, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    string example = "";
    example = "graph TD\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-utils:${version.plexus-utils}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-xml:${version.plexus-xml}\n    org.apache.mave:maven-parent:43 --> org.junit:junit-bom:${versions.junit5}\n";
    cout<<example<<endl;
    EXPECT_EQ(mermaid_code,example);
}

TEST(AddTest, HandlesPositiveInput_4) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.apache.maven:maven-parent:43" ,"C:/Users/jackt/conf_upr/second/", "2", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.mave";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, 2, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    string example = "graph TD\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-api:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-engine:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-migrationsupport:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-params:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-commons:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-console:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-engine:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-jfr:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-launcher:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-reporting:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-runner:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite-api:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite-commons:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite-engine:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-testkit:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.vintage:junit-vintage-engine:5.10.3\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.openjdk.jmh:jmh-core:1.37\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.openjdk.jmh:jmh-generator-annprocess:1.37\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.junit.jupiter:junit-jupiter:\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.codehaus.plexus:plexus-utils:4.0.1\n    org.codehaus.plexus:plexus-utils:${version.plexus-utils} --> org.codehaus.plexus:plexus-xml:3.0.0\n    org.codehaus.plexus:plexus-utils:${version.plexus-utils} --> org.junit.jupiter:junit-jupiter-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> com.google.inject:guice:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> javax.annotation:javax.annotation-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> javax.enterprise:cdi-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.eclipse.sisu:org.eclipse.sisu.inject:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-component-annotations:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-classworlds:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-utils:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-xml:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.slf4j:slf4j-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.osgi:osgi.core:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> junit:junit:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> ch.qos.logback:logback-classic:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> com.google.inject:guice:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.slf4j:slf4j-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.osgi:osgi.core:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> javax.annotation:javax.annotation-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> javax.enterprise:cdi-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> javax.servlet:servlet-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> com.google.inject.extensions:guice-servlet:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> junit:junit:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.junit.jupiter:junit-jupiter-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.testng:testng:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> ch.qos.logback:logback-classic:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> com.google.inject.extensions:guice-assistedinject:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.apache.felix:org.apache.felix.framework:\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-utils:${version.plexus-utils}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-xml:${version.plexus-xml}\n    org.apache.mave:maven-parent:43 --> org.junit:junit-bom:${versions.junit5}\n";
    cout<<example<<endl;
    EXPECT_EQ(mermaid_code,example);
}
