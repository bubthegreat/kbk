Current status: __ALIVE__

To Get things running:

Download the repo

`git clone https://github.com/bubthegreat/kbk.git`

Build the docker image:

`docker build -t kbkdocker .`

Start the image:

`docker run -dt -p 8989:8989 -v ${PWD}:/opt/kbk --name kbk_container kbkdocker`

Get in that baby real deep:

`docker exec -it kbk_container bash`

Restart the mysql server because it hates being installed for some reason:

`chown -R mysql:mysql /var/lib/mysql /var/run/mysqld `

`service mysql restart`

Make the database and users:

```
mysql -u root -proot -e "CREATE DATABASE kbkdatabase;"
mysql -u root -proot -e "CREATE USER kbkuser@localhost IDENTIFIED BY 'somepassword';"
mysql -u root -proot -e "GRANT ALL PRIVILEGES ON kbkdatabase.* TO 'someuser'@'localhost';"
mysql -u root -proot -e "FLUSH PRIVILEGES;"
```

Enter into the mysql command line:

`mysql -u root -proot`

Select the database:

`use kbkdatabase`;

Copypasta the database.sql file to create the actual database tables

```
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
```

build things (Go into the src directory for this)

`make -j8`

Start things up (Thi sis in the area directory)

`./startup`

