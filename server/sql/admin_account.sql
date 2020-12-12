USE swgemu;

--
-- Create admin account with highest privileges
-- user: admin
-- pass: admin
--
INSERT INTO `swgemu`.`accounts`
(`account_id`,`username`,`password`,`station_id`,`created`,`active`,`admin_level`,`salt`)
VALUES
('1', 'admin', 'e446b4224c335631832b417adf69246202c24db625cd7c10acae039e1bf1c8c0', '1987585210', '2019-11-30 07:06:52', '1', '15', '4a1111ad0729aa02edd33f4459477e5d');
