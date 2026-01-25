-- SQLite3 Database Schema for KBK MUD
-- Converted from MySQL schema

-- Bounties table - tracks player bounties
CREATE TABLE IF NOT EXISTS bounties (
  amount INTEGER NOT NULL DEFAULT 0,
  name TEXT NOT NULL DEFAULT '',
  victim TEXT NOT NULL DEFAULT '',
  ctime INTEGER NOT NULL DEFAULT 0,
  PRIMARY KEY (victim)
);

-- Logins table - login history (appears unused, kept for compatibility)
CREATE TABLE IF NOT EXISTS logins (
  name TEXT NOT NULL DEFAULT '',
  site TEXT NOT NULL DEFAULT '',
  logtime TEXT NOT NULL DEFAULT '',
  ctime INTEGER NOT NULL DEFAULT 0,
  type INTEGER NOT NULL DEFAULT 0,
  ip TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (name, ctime)
);

CREATE INDEX IF NOT EXISTS idx_logins_name ON logins(name);

-- Notes table - in-game notes/mail system
CREATE TABLE IF NOT EXISTS notes (
  type INTEGER NOT NULL DEFAULT 0,
  sender TEXT NOT NULL DEFAULT '',
  date TEXT NOT NULL DEFAULT '',
  to_list TEXT NOT NULL DEFAULT '',
  subject TEXT NOT NULL,
  text TEXT NOT NULL,
  timestamp INTEGER NOT NULL DEFAULT 0,
  PRIMARY KEY (type, sender, to_list, timestamp)
);

-- PK logs table - player kill tracking
CREATE TABLE IF NOT EXISTS pklogs (
  dead TEXT NOT NULL DEFAULT '',
  killer TEXT NOT NULL DEFAULT '',
  room TEXT NOT NULL DEFAULT '',
  time TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (dead, killer, time)
);

-- Traffic table - login/logout tracking
-- Note: IP stored as TEXT instead of MySQL's INET_ATON integer format
CREATE TABLE IF NOT EXISTS traffic (
  name TEXT NOT NULL DEFAULT '',
  ip TEXT NOT NULL DEFAULT '',
  hostname TEXT NOT NULL DEFAULT '',
  time TEXT NOT NULL DEFAULT (datetime('now')),
  type INTEGER NOT NULL DEFAULT 0,
  vnum INTEGER NOT NULL DEFAULT 0,
  PRIMARY KEY (name, type, time)
);

-- Player data table - cached player information
CREATE TABLE IF NOT EXISTS player_data (
  name TEXT NOT NULL DEFAULT '',
  level INTEGER NOT NULL DEFAULT 0,
  race INTEGER NOT NULL DEFAULT 0,
  class INTEGER NOT NULL DEFAULT 0,
  cabal INTEGER NOT NULL DEFAULT 0,
  sex INTEGER NOT NULL DEFAULT 0,
  alignment INTEGER NOT NULL DEFAULT 0,
  ethos TEXT NOT NULL DEFAULT '',
  ctime INTEGER NOT NULL DEFAULT 0,
  PRIMARY KEY (name)
);

-- Helpfiles table - help system (appears unused, kept for compatibility)
CREATE TABLE IF NOT EXISTS helpfiles (
  level INTEGER NOT NULL DEFAULT 0,
  keyword TEXT NOT NULL DEFAULT '',
  text TEXT,
  imm_only INTEGER NOT NULL DEFAULT 0
);

-- Player auth table - password storage
CREATE TABLE IF NOT EXISTS player_auth (
  name TEXT NOT NULL DEFAULT '',
  password TEXT NOT NULL DEFAULT '',
  level INTEGER NOT NULL DEFAULT 0,
  PRIMARY KEY (name)
);

-- Charmed mobs table - saved charmed/summoned creatures
CREATE TABLE IF NOT EXISTS charmed (
  owner TEXT NOT NULL DEFAULT '',
  vnum INTEGER NOT NULL DEFAULT 0,
  name TEXT,
  short_descr TEXT,
  long_descr TEXT,
  level INTEGER NOT NULL DEFAULT 0,
  max_hit INTEGER NOT NULL DEFAULT 0,
  hit INTEGER NOT NULL DEFAULT 0,
  alignment INTEGER NOT NULL DEFAULT 0,
  dice_number INTEGER NOT NULL DEFAULT 0,
  dice_type INTEGER NOT NULL DEFAULT 0,
  damroll INTEGER NOT NULL DEFAULT 0
);

-- Create indexes for common queries
CREATE INDEX IF NOT EXISTS idx_traffic_name ON traffic(name);
CREATE INDEX IF NOT EXISTS idx_traffic_time ON traffic(time);
CREATE INDEX IF NOT EXISTS idx_player_data_cabal ON player_data(cabal);
CREATE INDEX IF NOT EXISTS idx_charmed_owner ON charmed(owner);
CREATE INDEX IF NOT EXISTS idx_notes_type ON notes(type);
CREATE INDEX IF NOT EXISTS idx_pklogs_killer ON pklogs(killer);
CREATE INDEX IF NOT EXISTS idx_pklogs_dead ON pklogs(dead);

