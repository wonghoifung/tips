select d.Name, e.Name, e.Salary
from Department d join Employee e join (select max(Salary) as Salary,  DepartmentId as DepartmentId from Employee GROUP BY DepartmentId) h
where e.Salary = h.Salary and
e.DepartmentId = h.DepartmentId and
e.DepartmentId = d.Id
