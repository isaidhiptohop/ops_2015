## Copyright (C) 2016 phuksz
## 
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*- 
## @deftypefn {Function File} {@var{retval} =} nqueen (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author: phuksz <phuksz@X-Baby>
## Created: 2016-01-17

av1 = nqueens_9_1000_00_1 (2:nqueens_9_1000_00_1 (1,5), 1);
av2 = nqueens_9_1000_00_2 (2:nqueens_9_1000_00_2 (1,5), 1);
av3 = nqueens_9_1000_00_3 (2:nqueens_9_1000_00_3 (1,5), 1);
av4 = nqueens_9_1000_00_4 (2:nqueens_9_1000_00_4 (1,5), 1);
av5 = nqueens_9_1000_00_5 (2:nqueens_9_1000_00_5 (1,5), 1);

max_size = max ([size (av1), size (av2), size (av3), size (av4), size (av5)]);

x = [1:max_size];

plot (x, av1 (x), x, av2 (x), x, av3 (x), x, av4 (x), x, av5 (x));