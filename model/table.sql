DROP DATABASE IF EXISTS burgerChat;
CREATE DATABASE burgerChat DEFAULT CHARACTER SET utf8;
USE burgerChat;

-- name 考虑去unique
CREATE table User(
    id int NOT NULL AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    password VARCHAR(50) NOT NULL,
    state ENUM('online', 'offline') DEFAULT 'offline',
    UNIQUE(name),
    PRIMARY KEY(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE table Friend(
    userid INT NOT NULL,
    friendid INT NOT NULL,
    PRIMARY KEY(userid, friendid)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- name 考虑去unique
CREATE table AllGroup(
    id INT AUTO_INCREMENT,
    groupName VARCHAR(50) NOT NULL,
    groupDesc VARCHAR(200) DEFAULT '', 
    UNIQUE(groupName),
    PRIMARY KEY(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 多对多，要产生一个中间表
CREATE table GroupUser(
    groupid INT NOT NULL,
    userid INT NOT NULL,
    groupRole ENUM('creator', 'manager', 'normal') DEFAULT 'normal', 
    PRIMARY KEY(groupid, userid)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE table OfflineMsg(
    userid INT NOT NULL,
    message VARCHAR(500) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
