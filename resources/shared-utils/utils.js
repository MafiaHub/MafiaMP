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

// Export via Framework.Exports for cross-resource access
Framework.Exports.register("utils", utils);

module.exports = utils;
