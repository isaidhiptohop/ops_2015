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

data = nqueens_9_1000_00_1;

N = data (1,1);
pop_size = data (1,2);
m_p = data (1,3);
alpha = data (1,4);
size = data (1,5);
%size = 100;

av_fitness = data (2:size+1, 1);
peak_fitness = data (2:size+1, 8);
av_winner_fitness = data (2:size+1, 3);
peak_winner_fitness = data (2:size+1, 8);
av_postmutation_fitness = data (2:size+1, 5);
peak_postmutation_fitness = data (2:size+1, 8);
av_children_fitness = data (2:size+1, 7);
peak_children_fitness = data (2:size+1, 8);
mutations = data (2:size+1, 9);

plot (1:size, av_fitness, 1:size, av_winner_fitness);
figure;
plot (1:size, (av_winner_fitness./min(av_winner_fitness)), 
      1:size, (av_postmutation_fitness./min(av_postmutation_fitness)), 
      1:size, mutations./max(mutations));
