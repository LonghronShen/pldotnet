CREATE OR REPLACE FUNCTION retVarChar(fname varchar) RETURNS varchar AS $$
return fname + " Cabral";
$$ LANGUAGE pldotnet;
SELECT retVarChar('Rafael');

CREATE OR REPLACE FUNCTION retConcatVarChar(fname varchar, lname varchar) RETURNS varchar AS $$
return fname + lname;
$$ LANGUAGE pldotnet;
SELECT retConcatVarChar('João ', 'da Silva');

CREATE OR REPLACE FUNCTION retConcatText(fname text, lname text) RETURNS text AS $$
return "Hello " + fname + lname + "!";
$$ LANGUAGE pldotnet;
SELECT retConcatText('João ', 'da Silva');

CREATE OR REPLACE FUNCTION retVarCharText(fname varchar, lname varchar) RETURNS text AS $$
return "Hello " + fname + lname + "!";
$$ LANGUAGE pldotnet;
SELECT retVarCharText('Homer Jay ', 'Simpson');

CREATE OR REPLACE FUNCTION retChar(argchar character) RETURNS character AS $$
return argchar;
$$ LANGUAGE pldotnet;
SELECT retChar('R');

CREATE OR REPLACE FUNCTION retConcatLetters(a character, b character) RETURNS varchar AS $$
return a + b;
$$ LANGUAGE pldotnet;
SELECT retConcatLetters('R', 'C');

-- Problem here it is neither padding and truncating
CREATE OR REPLACE FUNCTION retConcatChars(a char(5), b char(7)) RETURNS char(5) AS $$
return a + b;
$$ LANGUAGE pldotnet;
SELECT retConcatChars('H.', 'Simpson');

CREATE OR REPLACE FUNCTION retConcatVarChars(a varchar(5), b varchar(7)) RETURNS varchar(5) AS $$
return a + b;
$$ LANGUAGE pldotnet;
SELECT retConcatVarChars('H.', 'Simpson');
SELECT retConcatVarChars('H. あ', 'Simpson');

CREATE OR REPLACE FUNCTION retNonRegularEncoding(a varchar) RETURNS varchar AS $$
return a;
$$ LANGUAGE pldotnet;
SELECT retNonRegularEncoding('漢字');
SELECT retNonRegularEncoding('ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃ');
SELECT retNonRegularEncoding('ŁĄŻĘĆŃŚŹ');
SELECT retNonRegularEncoding('Unicode, которая состоится 10-12 марта 1997 года в Майнце в Германии.');