
// g++ test.cpp -ltinyxml

// tutorial demo program
#include "stdio.h"
#include "tinyxml.h"

#include <iostream>

/****************************************/
int BuildSQLQuery(TiXmlNode* pParent) {
/****************************************/
  /* Sanity check */
  if(!pParent || !pParent->ToElement()) {
    return(-1);
  }

  /* Variable Declaration */
  int i = 0;
  TiXmlElement* pElement   = pParent->ToElement();
  TiXmlAttribute* pAttrib  = NULL;
  std::string sMemberName  = "";
  std::string sMemberValue = "";
  std::string sRet = "";

  /* Determine if there are any attributes to process */
  if((pAttrib = pElement->FirstAttribute()) == NULL) {
    return(-1);
  }

  /* Grab all of the attributes and build the string */
  for(i = 0; pAttrib; i++, pAttrib = pAttrib->Next()) {
    if(i > 0) {
      sMemberName  = sMemberName + ",";
      sMemberValue = sMemberValue + ",";
    }
    sMemberName  = sMemberName + std::string(pAttrib->Name());
    sMemberValue = sMemberValue + std::string("'") + std::string(pAttrib->Value()) + std::string("'");
  }

  /* Determine which SQL query to build */
  if(strcmp(pParent->Value(), "Statistics") >= 0) {
    sRet = std::string("INSERT INTO Statistics (") + sMemberName + std::string(") VALUES (") + sMemberValue + std::string(");");
  }
  else if(strcmp(pParent->Value(), "NodeConfig") >= 0) {
    sRet = std::string("REPLACE INTO NodeConfig (") + sMemberName + std::string(") VALUES (") + sMemberValue + std::string(");");
  }
  else if(strcmp(pParent->Value(), "Accounting") >= 0) {
    sRet = std::string("INSERT INTO NodeConfig (") + sMemberName + std::string(") VALUES (") + sMemberValue + std::string(");");
  }

  std::cout << sRet << std::endl;

  return(sRet.size());
}


/****************************************/
int ParseXml(TiXmlNode* pParent) {
/****************************************/
  /* Sanity check */
  if(!pParent) {
    return(-1);
  }

  /* Variable Declaration */
  TiXmlNode* pChild = NULL;

  /* Determine what type of the current XML node */
  switch(pParent->Type()) {
    /* Only parse the XML Element lines */
    case TiXmlNode::TINYXML_ELEMENT:
      BuildSQLQuery(pParent);
      break;

    /* Leaving these here in case they need to be parsed in the future */
    case TiXmlNode::TINYXML_TEXT:
    case TiXmlNode::TINYXML_DOCUMENT:
    case TiXmlNode::TINYXML_UNKNOWN:
    case TiXmlNode::TINYXML_DECLARATION:
    case TiXmlNode::TINYXML_COMMENT:
    default:
      break;
  }

  /* TODO - Should this be done without a recursive call? */
  /* Traverse the nodes */
  for(pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) {
    ParseXml(pChild);
  }

  return(0);
}


/****************************************/
void dump_to_stdout(const char* pFilename) {
/****************************************/
//  TiXmlDocument doc;
  TiXmlDocument doc(pFilename);

//  if(doc.Parse((const char*)filedata, 0, TIXML_ENCODING_UTF8)) {
  if(doc.LoadFile()) {
    ParseXml(&doc);
  }
  else {
    printf("Failed to load file \"%s\"\n", pFilename);
  }
}


/****************************************/
int main(int argc, char* argv[]) {
/****************************************/
  for (int i=1; i<argc; i++) {
    dump_to_stdout(argv[i]);
  }
  return 0;
}




