/**
 * Shared utility functions for MafiaMP resources.
 */

const utils = {
    getRandomInArray(arr) {
        if (!arr || arr.length === 0) return undefined;
        return arr[Math.floor(Math.random() * arr.length)];
    },

    getObjectKeys(obj) {
        return Object.keys(obj);
    }
};

// Register through the global Exports object for cross-resource access
Exports.register("utils", utils);

module.exports = utils;
