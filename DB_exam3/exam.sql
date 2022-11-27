-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- 主机： 127.0.0.1:3308
-- 生成日期： 2022-11-05 03:44:15
-- 服务器版本： 5.7.36
-- PHP 版本： 7.4.26

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 数据库： `exam`
--

-- --------------------------------------------------------

--
-- 表的结构 `customers`
--

DROP TABLE IF EXISTS `customers`;
CREATE TABLE IF NOT EXISTS `customers` (
  `cid` varchar(4) NOT NULL,
  `cname` varchar(15) DEFAULT NULL,
  `city` varchar(15) DEFAULT NULL,
  `visits_made` int(5) DEFAULT NULL,
  `last_visit_time` datetime DEFAULT NULL,
  PRIMARY KEY (`cid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 转存表中的数据 `customers`
--

INSERT INTO `customers` (`cid`, `cname`, `city`, `visits_made`, `last_visit_time`) VALUES
('c001', 'Brown', 'Binghamton', 1, '2017-12-05 09:12:30'),
('c002', 'Anne', 'Vestal', 1, '2018-11-29 14:30:00'),
('c003', 'Jack', 'Vestal', 1, '2018-12-04 16:48:02'),
('c004', 'Mike', 'Binghamton', 1, '2018-11-30 11:52:16'),
('c005', 'Wht', 'China', 2, '2022-11-02 00:00:00'),
('c006', 'SZ', 'SZU', 1, '2022-11-02 00:00:00'),
('c007', 'a222', 'bc', 1, '2022-11-02 00:00:00');

-- --------------------------------------------------------

--
-- 表的结构 `employees`
--

DROP TABLE IF EXISTS `employees`;
CREATE TABLE IF NOT EXISTS `employees` (
  `eid` varchar(3) NOT NULL,
  `ename` varchar(15) DEFAULT NULL,
  `city` varchar(15) DEFAULT NULL,
  PRIMARY KEY (`eid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 转存表中的数据 `employees`
--

INSERT INTO `employees` (`eid`, `ename`, `city`) VALUES
('e00', 'Amy', 'Vestal'),
('e01', 'Bob', 'Binghamton'),
('e02', 'John', 'Binghamton'),
('e03', 'Lisa', 'Binghamton'),
('e04', 'Matt', 'Vestal');

-- --------------------------------------------------------

--
-- 表的结构 `logs`
--

DROP TABLE IF EXISTS `logs`;
CREATE TABLE IF NOT EXISTS `logs` (
  `logid` int(5) NOT NULL AUTO_INCREMENT,
  `who` varchar(10) NOT NULL,
  `time` datetime NOT NULL,
  `table_name` varchar(20) NOT NULL,
  `operation` varchar(6) NOT NULL,
  `key_value` varchar(4) DEFAULT NULL,
  PRIMARY KEY (`logid`)
) ENGINE=InnoDB AUTO_INCREMENT=43 DEFAULT CHARSET=latin1;

--
-- 转存表中的数据 `logs`
--

INSERT INTO `logs` (`logid`, `who`, `time`, `table_name`, `operation`, `key_value`) VALUES
(32, 'super', '2022-11-02 11:06:14', 'suppliers', 'edit', '0'),
(33, 'super', '2022-11-02 11:06:40', 'suppliers', 'add', '0'),
(34, 'super', '2022-11-02 11:07:52', 'customers', 'edit', '0'),
(35, 'super', '2022-11-02 11:08:11', 'customers', 'add', '0'),
(36, 'super', '2022-11-02 11:12:02', 'customers', 'edit', '0'),
(37, 'super', '2022-11-02 11:13:59', 'suppliers', 'del', '0'),
(38, 'super', '2022-11-02 11:15:59', 'purchases', 'edit', '0'),
(39, 'super', '2022-11-02 11:16:46', 'customers', 'del', '0'),
(40, 'super', '2022-11-02 11:17:04', 'purchases', 'del', '0'),
(41, 'super', '2022-11-02 11:17:32', 'purchases', 'del', '0'),
(42, 'super', '2022-11-02 11:17:36', 'customers', 'del', '0');

-- --------------------------------------------------------

--
-- 表的结构 `products`
--

DROP TABLE IF EXISTS `products`;
CREATE TABLE IF NOT EXISTS `products` (
  `pid` varchar(4) NOT NULL,
  `pname` varchar(15) NOT NULL,
  `qoh` int(5) NOT NULL,
  `qoh_threshold` int(5) DEFAULT NULL,
  `original_price` decimal(6,2) DEFAULT NULL,
  `discnt_rate` decimal(3,2) DEFAULT NULL,
  `sid` varchar(2) DEFAULT NULL,
  PRIMARY KEY (`pid`),
  KEY `sid` (`sid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 转存表中的数据 `products`
--

INSERT INTO `products` (`pid`, `pname`, `qoh`, `qoh_threshold`, `original_price`, `discnt_rate`, `sid`) VALUES
('pr00', 'Milk', 12, 10, '2.40', '0.10', 's0'),
('pr01', 'Egg', 20, 10, '1.50', '0.20', 's1'),
('pr02', 'Bread', 15, 10, '1.20', '0.10', 's0'),
('pr03', 'Pineapple', 6, 5, '2.00', '0.30', 's0'),
('pr04', 'Knife', 10, 8, '2.50', '0.20', 's1'),
('pr05', 'Shovel', 5, 5, '7.99', '0.10', 's0');

-- --------------------------------------------------------

--
-- 表的结构 `purchases`
--

DROP TABLE IF EXISTS `purchases`;
CREATE TABLE IF NOT EXISTS `purchases` (
  `purid` int(11) NOT NULL,
  `cid` varchar(4) NOT NULL,
  `eid` varchar(3) NOT NULL,
  `pid` varchar(4) NOT NULL,
  `qty` int(5) DEFAULT NULL,
  `ptime` datetime DEFAULT NULL,
  `total_price` decimal(7,2) DEFAULT NULL,
  PRIMARY KEY (`purid`),
  KEY `cid` (`cid`),
  KEY `eid` (`eid`),
  KEY `pid` (`pid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 转存表中的数据 `purchases`
--

INSERT INTO `purchases` (`purid`, `cid`, `eid`, `pid`, `qty`, `ptime`, `total_price`) VALUES
(2, 'c001', 'e03', 'pr03', 2, '2018-12-05 09:12:30', '2.80'),
(3, 'c002', 'e03', 'pr00', 1, '2018-11-29 14:30:00', '2.16'),
(5, 'c004', 'e04', 'pr02', 3, '2018-11-30 11:52:16', '3.24'),
(6, 'c007', 'e02', 'pr05', 1, '2018-12-04 16:48:02', '7.19');

-- --------------------------------------------------------

--
-- 表的结构 `suppliers`
--

DROP TABLE IF EXISTS `suppliers`;
CREATE TABLE IF NOT EXISTS `suppliers` (
  `sid` varchar(2) NOT NULL,
  `sname` varchar(15) NOT NULL,
  `city` varchar(15) DEFAULT NULL,
  `telephone_no` char(10) DEFAULT NULL,
  PRIMARY KEY (`sid`),
  UNIQUE KEY `sname` (`sname`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 转存表中的数据 `suppliers`
--

INSERT INTO `suppliers` (`sid`, `sname`, `city`, `telephone_no`) VALUES
('s0', 'Supplier 1', 'Binghamton', '6075555431'),
('s1', 'Supplier 2', 'NYC', '6075555432'),
('s3', 'Supplier 3', 'China', '199934140');

--
-- 限制导出的表
--

--
-- 限制表 `products`
--
ALTER TABLE `products`
  ADD CONSTRAINT `products_ibfk_1` FOREIGN KEY (`sid`) REFERENCES `suppliers` (`sid`);

--
-- 限制表 `purchases`
--
ALTER TABLE `purchases`
  ADD CONSTRAINT `purchases_ibfk_1` FOREIGN KEY (`cid`) REFERENCES `customers` (`cid`),
  ADD CONSTRAINT `purchases_ibfk_2` FOREIGN KEY (`eid`) REFERENCES `employees` (`eid`),
  ADD CONSTRAINT `purchases_ibfk_3` FOREIGN KEY (`pid`) REFERENCES `products` (`pid`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
