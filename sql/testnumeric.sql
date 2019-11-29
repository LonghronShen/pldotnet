-- Test this only to check that PLSQL seems not to be 
-- caring of the precision/scale:
--CREATE OR REPLACE FUNCTION get_sum(
--   a NUMERIC(3,2), 
--   b NUMERIC) 
--RETURNS NUMERIC(4,1) AS $$
--BEGIN
--   RETURN a + b;
--END; $$
--LANGUAGE plpgsql;
--
--SELECT get_sum(2.3333333, 10);

CREATE OR REPLACE FUNCTION get_sum(
   a NUMERIC(3,2), 
   b NUMERIC) 
RETURNS NUMERIC(4,1) AS $$
return a + b;
$$
LANGUAGE plcsharp;
SELECT get_sum(1.3333333, 10) =  numeric '11.3333333';
SELECT get_sum(1.33333333, -10.99999999) = '-9.66666666';
SELECT get_sum(1999999999999.555555555555555, -10.99999999) = numeric '1999999999988.555555565555555'; -- 1999999999988.555555565555555

CREATE OR REPLACE FUNCTION getbigNum(a NUMERIC) RETURNS NUMERIC AS $$
return a;
$$
LANGUAGE plcsharp;
SELECT getbigNum(999999999999999999991.9999991) = numeric '999999999999999999991.9999991'; -- Sextllion at 7 scale (10 power 28 precision)
SELECT getbigNum(999999999999999999991.99999999) =  numeric '999999999999999999992.0000000'; -- It is rounded to 999999999999999999992.0000000



