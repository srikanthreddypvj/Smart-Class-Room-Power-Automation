CREATE TABLE "mytable" (
  "SELECT *" text
);

INSERT INTO "mytable" ("SELECT *")
VALUES
('FROM "wifi_status"'),
('WHERE'),
('time >= now() - interval ''1 day'''),
('SELECT *'),
('FROM "wifi_status"'),
('WHERE'),
('time >= now() - interval ''1 day'''),
('AND'),
('("Light1" IS NOT NULL OR "Light2" IS NOT NULL OR "Strength" IS NOT NULL OR "rssi" IS NOT NULL)'),
('SELECT *'),
('FROM "wifi_status"'),
('WHERE'),
('time >= now() - interval ''1 day'''),
('AND'),
('("Strength" IS NOT NULL OR "Light2" IS NOT NULL OR "Light1" IS NOT NULL OR "rssi" IS NOT NULL)');
