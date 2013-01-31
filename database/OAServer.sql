-- MySQL dump 10.13  Distrib 5.5.29, for Linux (i686)
--
-- Host: localhost    Database: OAServer
-- ------------------------------------------------------
-- Server version	5.5.29

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `OAAccounting`
--

DROP TABLE IF EXISTS `OAAccounting`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `OAAccounting` (
  `mIdx` int(11) NOT NULL AUTO_INCREMENT,
  `mCnt` int(11) NOT NULL,
  `sUsername` varchar(32) NOT NULL,
  `sGroupId` varchar(16) DEFAULT NULL,
  `sSystemId` varchar(16) DEFAULT NULL,
  `sNodeId` varchar(16) DEFAULT NULL,
  `dDate` datetime DEFAULT NULL,
  `fAmount` float NOT NULL,
  `sDescription` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`mIdx`),
  UNIQUE KEY `mIdx` (`mIdx`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `OAAccounting`
--

LOCK TABLES `OAAccounting` WRITE;
/*!40000 ALTER TABLE `OAAccounting` DISABLE KEYS */;
/*!40000 ALTER TABLE `OAAccounting` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `OAGroup`
--

DROP TABLE IF EXISTS `OAGroup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `OAGroup` (
  `sGroupId` varchar(16) NOT NULL,
  `sDescription` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`sGroupId`),
  UNIQUE KEY `sGroupId` (`sGroupId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `OAGroup`
--

LOCK TABLES `OAGroup` WRITE;
/*!40000 ALTER TABLE `OAGroup` DISABLE KEYS */;
/*!40000 ALTER TABLE `OAGroup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `OANodeCfg`
--

DROP TABLE IF EXISTS `OANodeCfg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `OANodeCfg` (
  `sUsername` varchar(32) NOT NULL,
  `sSystemId` varchar(16) NOT NULL,
  `sNodeId` varchar(16) NOT NULL,
  `sChannelNames` varchar(256) DEFAULT NULL,
  `sChannelUnits` varchar(64) DEFAULT NULL,
  `mPollingPeriod` int(11) NOT NULL DEFAULT '60',
  `sDescription` varchar(256) DEFAULT NULL,
  `bPublic` tinyint(1) NOT NULL DEFAULT '1',
  `bEnable` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`sNodeId`),
  UNIQUE KEY `sNodeId` (`sNodeId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `OANodeCfg`
--

LOCK TABLES `OANodeCfg` WRITE;
/*!40000 ALTER TABLE `OANodeCfg` DISABLE KEYS */;
/*!40000 ALTER TABLE `OANodeCfg` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `OASystemCfg`
--

DROP TABLE IF EXISTS `OASystemCfg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `OASystemCfg` (
  `sUsername` varchar(32) NOT NULL,
  `sSystemId` varchar(16) NOT NULL,
  `sGroupId` varchar(16) DEFAULT NULL,
  `sDescription` varchar(256) DEFAULT NULL,
  `bPublic` tinyint(1) NOT NULL DEFAULT '1',
  `bEnable` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`sSystemId`),
  UNIQUE KEY `sSystemId` (`sSystemId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `OASystemCfg`
--

LOCK TABLES `OASystemCfg` WRITE;
/*!40000 ALTER TABLE `OASystemCfg` DISABLE KEYS */;
/*!40000 ALTER TABLE `OASystemCfg` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `OAUserInfo`
--

DROP TABLE IF EXISTS `OAUserInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `OAUserInfo` (
  `sUsername` varchar(32) NOT NULL,
  `sEmail` varchar(64) NOT NULL,
  `sFirstName` varchar(32) NOT NULL,
  `sLastName` varchar(32) DEFAULT NULL,
  `sAddress` varchar(128) DEFAULT NULL,
  `sAddress2` varchar(128) DEFAULT NULL,
  `sCity` varchar(32) DEFAULT NULL,
  `sState` varchar(32) DEFAULT NULL,
  `sZip` varchar(16) DEFAULT NULL,
  `sPhone` varchar(16) DEFAULT NULL,
  `bPublic` tinyint(1) NOT NULL DEFAULT '1',
  `bEnable` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`sUsername`),
  UNIQUE KEY `sUsername` (`sUsername`),
  UNIQUE KEY `sEmail` (`sEmail`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `OAUserInfo`
--

LOCK TABLES `OAUserInfo` WRITE;
/*!40000 ALTER TABLE `OAUserInfo` DISABLE KEYS */;
/*!40000 ALTER TABLE `OAUserInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `OAUserPass`
--

DROP TABLE IF EXISTS `OAUserPass`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `OAUserPass` (
  `sUsername` varchar(32) NOT NULL,
  `sPassword` varchar(32) NOT NULL,
  PRIMARY KEY (`sUsername`),
  UNIQUE KEY `sUsername` (`sUsername`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `OAUserPass`
--

LOCK TABLES `OAUserPass` WRITE;
/*!40000 ALTER TABLE `OAUserPass` DISABLE KEYS */;
/*!40000 ALTER TABLE `OAUserPass` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-01-26 11:06:12
