program hanoi;
var n;

procedure hanoi(a,b,c,n);
var i;
if n = 1 then write a,b
else
begin
	hanoi(1,2,3,1);
   write a,b
end;

begin
   n := 5;
   hanoi(1,2,3,n)
end.
