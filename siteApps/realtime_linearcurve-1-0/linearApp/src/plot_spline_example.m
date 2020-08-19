arg_list = argv();
ref_csv = arg_list{1};
spline_csv = arg_list{2};
ref_data = csvread(ref_csv, 0, 0);
spl_data = csvread(spline_csv, 0, 0);

figure();
hold on;
plot(ref_data(:, 1), ref_data(:, 2), "r.", "markersize", 20);
plot(spl_data(:, 1), spl_data(:, 2), "b-", "linewidth", 2.0);
xlabel("Time [s]");
ylabel("Response");
ginput()
