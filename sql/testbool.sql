CREATE OR REPLACE FUNCTION returnBool() RETURNS boolean AS $$
return false;
$$ LANGUAGE pldotnet;
SELECT returnBool() is false;

CREATE OR REPLACE FUNCTION BooleanAnd(a boolean, b boolean) RETURNS boolean AS $$
return a&b;
$$ LANGUAGE pldotnet;
SELECT BooleanAnd(true, true) is true;

CREATE OR REPLACE FUNCTION BooleanOr(a boolean, b boolean) RETURNS boolean AS $$
return a|b;
$$ LANGUAGE pldotnet;
SELECT BooleanOr(false, false) is false;

CREATE OR REPLACE FUNCTION BooleanXor(a boolean, b boolean) RETURNS boolean AS $$
return a^b;
$$ LANGUAGE pldotnet;
SELECT BooleanXor(false, false) is false;

