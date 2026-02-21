#pragma once

struct testResult {
    int totalTests = 0;
    int failedTests = 0;
    int expectedFail = 0;

    testResult & operator+=(const testResult& o) {
        totalTests += o.totalTests;
        failedTests += o.failedTests;
        expectedFail += o.expectedFail;
        return *this;
    }
};