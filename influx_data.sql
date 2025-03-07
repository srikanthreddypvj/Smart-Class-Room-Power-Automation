CREATE TABLE "mytable" (
  "SELECT *" text
);
--Inserting from the ESP32

INSERT INTO "mytable" ("SELECT *")
VALUES
('FROM "wifi_status"'),
('WHERE'),
('time >= now() - interval ''1 minute'''),
('AND'),
('("Strength" IS NOT NULL OR "Light1" IS NOT NULL OR "Light2" IS NOT NULL OR "rssi" IS NOT NULL)'),
('SELECT *'),
('FROM "wifi_status"'),
('WHERE'),
('time >= now() - interval ''2 days'''),
('AND'),
('("Strength" IS NOT NULL)'),
('SELECT *'),
('FROM "wifi_status"'),
('WHERE'),
('time >= now() - interval ''2 days'''),
('AND'),
('("Strength" IS NOT NULL)'),
('SELECT *'),
('FROM "wifi_status"'),
('WHERE'),
('time >= now() - interval ''2 days'''),
('AND'),
('("Strength" IS NOT NULL)');


