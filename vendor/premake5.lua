function files_prefix(prefix)
    return function(_files)
        for k, v in ipairs(_files) do
            _files[k] = prefix .. "/" .. v
        end

        return files(_files)
    end
end

function removefiles_prefix(prefix)
    return function(_files)
        for k, v in ipairs(_files) do
            _files[k] = prefix .. "/" .. v
        end

        return removefiles(_files)
    end
end

local submodules_list = {}
for _, path in pairs(os.matchfiles("submodules/*.lua")) do
    local submodule = dofile(path)
    submodule.name = string.sub(path, string.find(path, "/[^/]*$") + 1, string.find(path, ".[^.]*$") - 1) -- extract filename
    print("Found submodule: " .. submodule.name)
    table.insert(submodules_list, submodule)
end

local vendor_cwd = os.getcwd()

function submodules(list)
    if type(list) ~= "table" then
        list = { list }
    end

    local old_cwd = os.getcwd()
    os.chdir(vendor_cwd .. "/submodules")

    for _, submodule in pairs(submodules_list) do
        for _, submodule_name in pairs(list) do
            if submodule.name == submodule_name then
                submodule.include()

                if submodule.project then
                    links(submodule.name)
                end
                break
            end
        end
    end

    os.chdir(old_cwd)
end

os.chdir("submodules") -- tidy up a bit generated files

group "vendor"

for _, submodule in pairs(submodules_list) do
    if submodule.project then
        project(submodule.name)

        warnings "Off"
        
        submodule.include()
        submodule.project()
    end
end

group ""

os.chdir(vendor_cwd)
