CREATE OR REPLACE FUNCTION ageTest(name varchar, age integer, lname varchar) RETURNS varchar AS $$
FormattableString res;
if (age < 18)
    res = $" Hey {name} {lname}! Dude you are still a kid.";
else if (age >= 18 && age < 40)
    res = $" Hey {name} {lname}! You are in the mood!";
else
    res = $" Hey {name} {lname}! You are getting experienced!";
return res.ToString();
$$ LANGUAGE pldotnet;

SELECT ageTest('Billy', 10, 'The KID');
SELECT ageTest('John', 33, 'Smith');
SELECT ageTest('Robson', 41, 'Cruzoe');

