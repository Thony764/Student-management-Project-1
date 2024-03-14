-- Crear tabla 'grado' para almacenar el número de grado
CREATE TABLE grado
(
    num_grad INTEGER PRIMARY KEY
);

-- Insertar valores de grado del 1 al 6
INSERT INTO grado (num_grad) VALUES 
(1),
(2),
(3),
(4),
(5),
(6);

-- Crear tabla 'alumno' para almacenar información del alumno
CREATE TABLE alumno
(
    id_alu INTEGER PRIMARY KEY AUTOINCREMENT,
    dni_alu TEXT,
    nombre_alu TEXT,
    apellidoP_alu TEXT,
    apellidoM_alu TEXT,
    edad_alu TEXT,
    num_grad2 INTEGER,
    dni_apo TEXT,
    
    FOREIGN KEY(num_grad2) REFERENCES grado (num_grad)
);

-- Crear tabla 'rememberme_status' para almacenar el estado de recordarme
CREATE TABLE rememberme_status (
    id_rememberme INTEGER
);

-- Insertar valor inicial en la tabla rememberme_status
INSERT INTO rememberme_status (id_rememberme) VALUES (0);

-- Crear tabla 'user' para almacenar información del usuario
CREATE TABLE "user" (  
    "id_user" INTEGER,  
    "username" TEXT, 
    "password" INTEGER, 
    "gmail" TEXT, 
    PRIMARY KEY("id_user" AUTOINCREMENT)
);

-- Crear tabla 'settings' para almacenar configuraciones
CREATE TABLE settings (
    precio_matricula REAL,
    num_vacantes INT,
    auto_refresh_tb INT,
    edit_tb_mode INT,
    hide_DB_status_bar INT
);

-- Insertar valores iniciales en la tabla settings
INSERT INTO settings (precio_matricula, num_vacantes, auto_refresh_tb, edit_tb_mode, hide_DB_status_bar)
VALUES (25.0, 100, 0, 0, 0);

-- Insertar datos de ejemplo en la tabla 'alumno'
INSERT INTO alumno (dni_alu, nombre_alu, apellidoP_alu, apellidoM_alu, edad_alu, num_grad2, dni_apo)
VALUES 
('12345678', 'Juan', 'Perez', 'Gonzalez', '8', 1, '87654321'),
('23456789', 'Maria', 'Garcia', 'Lopez', '7', 2, '98765432'),
('34567890', 'Luis', 'Martinez', 'Fernandez', '6', 3, '09876543'),
('45678901', 'Ana', 'Rodriguez', 'Sanchez', '9', 4, '10987654'),
('56789012', 'Pedro', 'Lopez', 'Gomez', '10', 5, '21098765'),
('67890123', 'Laura', 'Gomez', 'Perez', '5', 6, '32109876'),
('78901234', 'Pablo', 'Fernandez', 'Martinez', '8', 1, '43210987'),
('89012345', 'Sofia', 'Sanchez', 'Garcia', '7', 2, '54321098'),
('90123456', 'Carlos', 'Gonzalez', 'Lopez', '6', 3, '65432109'),
('01234567', 'Elena', 'Perez', 'Martinez', '9', 4, '76543210'),
('12345098', 'Miguel', 'Gomez', 'Rodriguez', '10', 5, '87654321'),
('23456701', 'Alicia', 'Lopez', 'Fernandez', '5', 6, '98765432'),
('34567012', 'Javier', 'Garcia', 'Sanchez', '8', 1, '09876543'),
('45670123', 'Beatriz', 'Martinez', 'Gonzalez', '7', 2, '10987654'),
('56701234', 'Diego', 'Sanchez', 'Lopez', '6', 3, '21098765'),
('67012345', 'Lucia', 'Rodriguez', 'Gomez', '9', 4, '32109876'),
('70123456', 'Marta', 'Lopez', 'Perez', '10', 5, '43210987'),
('71234567', 'Jorge', 'Fernandez', 'Garcia', '5', 6, '54321098'),
('72345678', 'Isabel', 'Gomez', 'Martinez', '8', 1, '65432109'),
('73456789', 'Alberto', 'Perez', 'Rodriguez', '7', 2, '76543210'),
('74567890', 'Carmen', 'Gonzalez', 'Fernandez', '6', 3, '87654321'),
('75678901', 'Pilar', 'Sanchez', 'Lopez', '9', 4, '98765432'),
('76789012', 'Raul', 'Lopez', 'Gomez', '10', 5, '09876543'),
('77890123', 'Sara', 'Garcia', 'Perez', '5', 6, '10987654'),
('78901234', 'Manuel', 'Martinez', 'Gonzalez', '8', 1, '21098765'),
('79012345', 'Natalia', 'Sanchez', 'Lopez', '7', 2, '32109876'),
('70123456', 'Luisa', 'Rodriguez', 'Gomez', '6', 3, '43210987'),
('71234567', 'Antonio', 'Lopez', 'Fernandez', '9', 4, '54321098'),
('72345678', 'Eva', 'Garcia', 'Martinez', '10', 5, '65432109')
;
