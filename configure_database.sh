echo -e  "\nStep 3: Starting and configuring MySQL Server."
MYSQL_INSTALL=$(expect -c "
set timeout 10
spawn apt-get install -y mysql-server
expect \"New password for the MySQL "root" user:\"
send \"${password}\r\"
expect \"Repeat password for the MySQL "root" user:\"
send \"${password}\r\"
expect eof
")


echo "$MYSQL_INSTALL" &>> ./install.log || stop_install
sleep 10;
service mysql start &>> ./install.log || stop_install
# Give mysql some time to start
sleep 10;
/usr/bin/mysql -h 0.0.0.0 -P 3306 --protocol=tcp -u root --password=${password} -e "CREATE DATABASE IF NOT EXISTS flash_stache; SET @@global.time_zone = '+00:00';" &>> ./install.log || stop_install

CREATE TABLE `bounties` (
  `amount` int(10) NOT NULL default '0',
  `name` varchar(25) NOT NULL default '',
  `victim` varchar(25) NOT NULL default '',
  `ctime` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`victim`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `logins` (
  `name` varchar(50) NOT NULL default '',
  `site` varchar(255) NOT NULL default '',
  `logtime` varchar(255) NOT NULL default '',
  `ctime` int(10) unsigned NOT NULL default '0',
  `type` tinyint(1) NOT NULL default '0',
  `ip` varchar(15) NOT NULL default '',
  PRIMARY KEY  (`name`,`ctime`),
  KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `notes` (
  `type` tinyint(1) NOT NULL default '0',
  `sender` varchar(100) NOT NULL default '',
  `date` varchar(50) NOT NULL default '',
  `to_list` varchar(100) NOT NULL default '',
  `subject` text NOT NULL,
  `text` text NOT NULL,
  `timestamp` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`type`,`sender`,`to_list`,`timestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `pklogs` (
  `dead` varchar(25) NOT NULL default '',
  `killer` varchar(25) NOT NULL default '',
  `room` varchar(255) NOT NULL default '',
  `time` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`dead`,`killer`,`time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `traffic` (
  `name` varchar(30) NOT NULL default '',
  `ip` int(10) unsigned NOT NULL default '0',
  `hostname` varchar(255) NOT NULL default '',
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `type` tinyint(1) NOT NULL default '0',
  `vnum` mediumint(8) NOT NULL default '0',
  PRIMARY KEY  (`name`,`type`,`time`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
