load keyTimes
clf
plot(f(:,1)); hold on;
startTimes = keyTimes(1:2:end);
endTimes = keyTimes(2:2:end);
for i = 1 : length(startTimes); plot([startTimes(i); startTimes(i)], [1e9,-1], '-g', 'LineWidth', 3); hold on; end;
for i = 1 : length(endTimes); plot([endTimes(i); endTimes(i)], [1e9,-1e2], '-r', 'LineWidth', 3); hold on; end;










return;
y1 = find(y==1);
y2 = find(y==2);
y3 = find(y==3);
y4 = find(y==4);
fL1 = fL(y1,1);
fL2 = fL(y2,1);
fL3 = fL(y3,1);
fL4 = fL(y4,1);
plot(f(:,1), 'k'); hold on;
plot(L, fL(:,1), 'm'); hold on;
pause
plot(L(y1), fL1, 'r'); hold on;
plot(L(y2), fL2, 'g'); hold on;
plot(L(y3), fL3, 'b'); hold on;
plot(L(y4), fL4, 'c'); hold on;
