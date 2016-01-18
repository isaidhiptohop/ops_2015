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

load nqueens_9_1000_00_1;
load nqueens_9_1000_00_2;
load nqueens_9_1000_00_3;
load nqueens_9_1000_00_4;
load nqueens_9_1000_00_5;

av1 = nqueens_9_1000_00_1 (2:nqueens_9_1000_00_1 (1,5)+1, 1);
av2 = nqueens_9_1000_00_2 (2:nqueens_9_1000_00_2 (1,5)+1, 1);
av3 = nqueens_9_1000_00_3 (2:nqueens_9_1000_00_3 (1,5)+1, 1);
av4 = nqueens_9_1000_00_4 (2:nqueens_9_1000_00_4 (1,5)+1, 1);
av5 = nqueens_9_1000_00_5 (2:nqueens_9_1000_00_5 (1,5)+1, 1);

min_size = min ([size(av1, 1), 
                 size(av2, 1), 
                 size(av3, 1), 
                 size(av4, 1), 
                 size(av5, 1)]);

x = [1:min_size];

figure;
plot (x, av1 (x), x, av2 (x), x, av3 (x), x, av4 (x), x, av5 (x));

load nqueens_9_1000_010_2;
load nqueens_9_1000_029_2;
load nqueens_9_1000_059_2;
load nqueens_9_1000_0100_2;

av_9_0 = nqueens_9_1000_010_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,1);
av_9_1 = nqueens_9_1000_029_2 ( 2:nqueens_9_1000_029_2 (1,5)+1 ,1);
av_9_2 = nqueens_9_1000_059_2 ( 2:nqueens_9_1000_059_2 (1,5)+1 ,1);
av_9_3 = nqueens_9_1000_0100_2 ( 2:nqueens_9_1000_0100_2 (1,5)+1 ,1);

min_size = min ([nqueens_9_1000_010_2(1,5), 
                 nqueens_9_1000_029_2(1,5), 
                 nqueens_9_1000_059_2(1,5), 
                 nqueens_9_1000_0100_2(1,5)]);

x = [1:min_size];

av_w_9_0 = nqueens_9_1000_010_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,3);
av_w_9_1 = nqueens_9_1000_029_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,3);
av_w_9_2 = nqueens_9_1000_059_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,3);
av_w_9_3 = nqueens_9_1000_0100_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,3);

av_pm_9_0 = nqueens_9_1000_010_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,5);
av_pm_9_1 = nqueens_9_1000_029_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,5);
av_pm_9_2 = nqueens_9_1000_059_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,5);
av_pm_9_3 = nqueens_9_1000_0100_2 ( 2:nqueens_9_1000_010_2 (1,5)+1 ,5);

mut_9_0 = nqueens_9_1000_010_2 (2:nqueens_9_1000_010_2 (1,5)+1, 9);
mut_9_0 = mut_9_0 / max (mut_9_0);
mut_9_1 = nqueens_9_1000_029_2 (2:nqueens_9_1000_029_2 (1,5)+1, 9);
mut_9_1 = mut_9_1 / max (mut_9_1);
mut_9_2 = nqueens_9_1000_059_2 (2:nqueens_9_1000_059_2 (1,5)+1, 9);
mut_9_2 = mut_9_2 / max (mut_9_2);
mut_9_3 = nqueens_9_1000_0100_2 (2:nqueens_9_1000_0100_2 (1,5)+1, 9);
mut_9_3 = mut_9_3 / max (mut_9_3);

figure;
plot (x, av_9_0 (x), x, av_9_1 (x), x, av_9_2 (x), x, av_9_3 (x));

figure;
plot (x, av_w_9_0 (x), x, av_pm_9_0 (x), x, mut_9_0 (x));
figure;
plot (x, av_w_9_1 (x), x, av_pm_9_1 (x), x, mut_9_1 (x));
figure;
plot (x, av_w_9_2 (x), x, av_pm_9_2 (x), x, mut_9_2 (x));
figure;
plot (x, av_w_9_3 (x), x, av_pm_9_3 (x), x, mut_9_3 (x));
