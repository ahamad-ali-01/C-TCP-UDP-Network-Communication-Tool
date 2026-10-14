# Testing guide

Run `./scripts/run_tests.sh` or build and run `ctest --test-dir build --output-on-failure`. Unit tests check serialization, malformed headers/lengths, payload bounds, and metric calculations. Integration tests spawn local TCP and UDP servers, test echo and benchmark paths, check generated CSV/JSON, and start three concurrent TCP clients. They require available localhost ports 19450 and 19451 and use a 30-second CTest timeout.
