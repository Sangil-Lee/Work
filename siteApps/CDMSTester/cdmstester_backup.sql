-- MariaDB dump 10.19  Distrib 10.5.12-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: cdmstester
-- ------------------------------------------------------
-- Server version	10.5.12-MariaDB-0+deb11u1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `module_t`
--

DROP TABLE IF EXISTS `module_t`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `module_t` (
  `sernum` varchar(64) NOT NULL,
  `modname` varchar(128) DEFAULT NULL,
  `modtype` tinyint(3) unsigned DEFAULT NULL,
  `location` tinyint(3) unsigned DEFAULT NULL,
  `desciption` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`sernum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `module_t`
--

LOCK TABLES `module_t` WRITE;
/*!40000 ALTER TABLE `module_t` DISABLE KEYS */;
INSERT INTO `module_t` VALUES ('833-3434','RTP-93483',1,0,'RTP Analog Output'),('833-3438','RTP-93483',0,1,'RTP Analog Input'),('863-3438','RTP-9343',2,0,'RTP Digital Input'),('934-9342','PXI-9239A',3,1,'Digital Input'),('RTP-3423','RTP-8349',1,0,'Analog Input');
/*!40000 ALTER TABLE `module_t` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `result_t`
--

DROP TABLE IF EXISTS `result_t`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `result_t` (
  `ridx` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `tdate` datetime DEFAULT NULL,
  `result` tinyint(3) unsigned DEFAULT NULL,
  `rvalue` double DEFAULT NULL,
  `sidx` int(10) unsigned DEFAULT NULL,
  `sernum` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`ridx`),
  KEY `sidx` (`sidx`),
  KEY `sernum` (`sernum`),
  CONSTRAINT `result_t_ibfk_1` FOREIGN KEY (`sidx`) REFERENCES `scenario_t` (`sidx`),
  CONSTRAINT `result_t_ibfk_2` FOREIGN KEY (`sernum`) REFERENCES `module_t` (`sernum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `result_t`
--

LOCK TABLES `result_t` WRITE;
/*!40000 ALTER TABLE `result_t` DISABLE KEYS */;
/*!40000 ALTER TABLE `result_t` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scenario_t`
--

DROP TABLE IF EXISTS `scenario_t`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scenario_t` (
  `sidx` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `scenario` varchar(1024) DEFAULT NULL,
  `modinfo` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`sidx`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scenario_t`
--

LOCK TABLES `scenario_t` WRITE;
/*!40000 ALTER TABLE `scenario_t` DISABLE KEYS */;
INSERT INTO `scenario_t` VALUES (1,'TEST(RTP-Mod00:AI-CH00:Volt),SCAN(0.1),FUNC(Auto,Sine Wave,Min=1,Max=3)',''),(2,'TEST(RTP-Mod00:AI-CH01:Volt),SCAN(5),FUNC(Auto,Rectangle,Min=1,Max=3)',''),(3,'TEST(RTP-Mod00:DI-CH00:Volt),SCAN(10),FUNC(Manu,24)',''),(4,'TEST(RTP-Mod00:AI-CH00:Volt),SCAN(0.1),FUNC(Auto,Sine Wave,Min=0,Max=5)',''),(5,'TEST(RTP-Mod00:AI-CH01:Volt),SCAN(0.5),FUNC(Auto,Sine Wave,Min=0,Max=2),ModInfo(RTP-3423)',''),(6,'TEST(RTP-Mod00:AI-CH00:Volt),SCAN(0.1),FUNC(Auto,Sine Wave,Min=0,Max=1)','RTP-3423'),(7,'TEST(RTP-Mod00:AI-CH00:Volt),SCAN(0.1),FUNC(Auto,Sine Wave,Min=0,Max=1)',''),(8,'TEST(RTP-Mod00:AI-CH00:Volt),SCAN(0.1),FUNC(Auto,Sine Wave,Min=0,Max=1)','934-9342'),(9,'TEST(RTP-Mod00:AI-CH00:Volt),SCAN(0.1),FUNC(Auto,Sine Wave,Min=0,Max=1)','833-3438'),(10,'TEST(RTP-Mod00:AI-CH00:Volt),SCAN(0.1),FUNC(Auto,Sine Wave,Min=0,Max=4)','863-3438');
/*!40000 ALTER TABLE `scenario_t` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_t`
--

DROP TABLE IF EXISTS `user_t`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_t` (
  `seq` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` varchar(20) DEFAULT NULL,
  `passwd` varchar(50) DEFAULT NULL,
  `grp` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`seq`)
) ENGINE=MyISAM AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_t`
--

LOCK TABLES `user_t` WRITE;
/*!40000 ALTER TABLE `user_t` DISABLE KEYS */;
INSERT INTO `user_t` VALUES (1,'silee7103','qwer1234','admin'),(2,'silee','123456','engineer'),(7,'user1','1234','engineer'),(8,'oper1','1234','operator'),(6,'ctrluser','qwer1234','admin');
/*!40000 ALTER TABLE `user_t` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-09-30 17:06:30
