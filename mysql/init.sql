CREATE DATABASE IF NOT EXISTS `kbkdatabase`;
USE `kbkdatabase`;

CREATE TABLE IF NOT EXISTS `bounties` (
  `amount` int(10) NOT NULL default '0',
  `name` varchar(25) NOT NULL default '',
  `victim` varchar(25) NOT NULL default '',
  `ctime` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`victim`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS  `logins` (
  `name` varchar(50) NOT NULL default '',
  `site` varchar(255) NOT NULL default '',
  `logtime` varchar(255) NOT NULL default '',
  `ctime` int(10) unsigned NOT NULL default '0',
  `type` tinyint(1) NOT NULL default '0',
  `ip` varchar(15) NOT NULL default '',
  PRIMARY KEY  (`name`,`ctime`),
  KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS  `notes` (
  `type` tinyint(1) NOT NULL default '0',
  `sender` varchar(100) NOT NULL default '',
  `date` varchar(50) NOT NULL default '',
  `to_list` varchar(100) NOT NULL default '',
  `subject` text NOT NULL,
  `text` text NOT NULL,
  `timestamp` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`type`,`sender`,`to_list`,`timestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS  `pklogs` (
  `dead` varchar(25) NOT NULL default '',
  `killer` varchar(25) NOT NULL default '',
  `room` varchar(255) NOT NULL default '',
  `time` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`dead`,`killer`,`time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS  `traffic` (
  `name` varchar(30) NOT NULL default '',
  `ip` int(10) unsigned NOT NULL default '0',
  `hostname` varchar(255) NOT NULL default '',
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `type` tinyint(1) NOT NULL default '0',
  `vnum` mediumint(8) NOT NULL default '0',
  PRIMARY KEY  (`name`,`type`,`time`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `player_data` (
  `name` varchar(25) NOT NULL default '',
  `level` int(10) NOT NULL default '0',
  `race` int(10) NOT NULL default '0',
  `class` int(10) NOT NULL default '0',
  `cabal` int(10) NOT NULL default '0',
  `sex` int(10) NOT NULL default '0',
  `alignment` int(10) NOT NULL default '0',
  `ethos` varchar(25) NOT NULL default '',
  `ctime` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `helpfiles` (
  `level` int(10) NOT NULL default '0',
  `keyword` varchar(25) NOT NULL default '',
  `text` text,
  `imm_only` bool NOT NULL default false
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `player_auth` (
  `name` varchar(25) NOT NULL default '',
  `password` varchar(25) NOT NULL default '',
  `level` int(10) NOT NULL default '0',
  PRIMARY KEY  (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `charmed` (
  `owner` varchar(25) NOT NULL default '',
  `vnum` int(10) NOT NULL default '0',
  `short_descr` text,
  `long_descr` text,
  `level` int(10) NOT NULL default '0',
  `hit` int(10) NOT NULL default '0',
  `alignment` int(10) NOT NULL default '0',
  `dice_number` int(10) NOT NULL default '0',
  `dice_type` int(10) NOT NULL default '0',
  `damroll` int(10) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- Create user with mysql_native_password for compatibility with old clients
-- MySQL 8.4 still supports it, just not as default
CREATE USER IF NOT EXISTS 'kbkuser'@'%' IDENTIFIED WITH mysql_native_password BY 'kbkpassword';
GRANT ALL ON `kbkdatabase`.* to 'kbkuser'@'%';
FLUSH PRIVILEGES;
