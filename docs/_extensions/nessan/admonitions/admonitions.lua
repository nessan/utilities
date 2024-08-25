--[[
Quarto extension to process AsciiDoc style 'admonitions'
See: https://docs.asciidoctor.org/asciidoc/latest/blocks/admonitions/
For now we only support paragraph syntax for admonitions and only have HTML output.
SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
SPDX-License-Identifier: MIT
--]]

-- Here are the admonition styles we recognize.
-- For example, a paragraph starting with 'NOTE: ' will trigger a NOTE style admonition.
local admonitions = {
    NOTE        = { name = "note",      title = "Note"},
    TIP         = { name = "tip",       title = "Tip" },
    WARNING     = { name = "warning",   title = "Warning" },
    CAUTION     = { name = "caution",   title = "Caution" },
    IMPORTANT   = { name = "important", title = "Important" }
}

-- We process each admonition into an one row, two cell HTML table decorated with CSS classes etc.
-- Need to inject the the appropriate links and CSS for those to get those tables rendered correctly.
local need_html_dependencies = true
local function process_admonition_dependencies()
    if need_html_dependencies then
        -- Inject a link to the fontawesome stylesheet (we use some of their icons to mark the admonition)
        local fontawesome_link = '<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css"/>'
        quarto.doc.include_text('in-header', fontawesome_link)

        -- Inject our own CSS for styling admonition content and icons -- we have those CSS rules in a local stylesheet.
        quarto.doc.add_html_dependency({
            name = 'admonitions',
            version = '1.0.0',
            stylesheets = {'assets/admonitions.css'}
        })

        -- Don't need to call this again
        need_html_dependencies = false
    end
end

-- Given an admonition style and its content we create an apporiate HTML table to insert in its place.
local function process_admonition(admonition, content)

    -- Add the HTML dependencies if we need to.
    if need_html_dependencies then process_admonition_dependencies() end

    -- Create an HTML div containing a table with one row and two cells [icon, content].
    -- The div & table cells are decorated with classes that depend on the particular admonition style.
    -- The HTML for that looks like the following where we use placeholders for the class names etc.
    local pre_template = [[<div class="admonition {{<name>}}">
    <table><tr>
    <td class="icon"> <i class="fa icon-{{<name>}}" title="{{<Title>}}"></i></td>
    <td class="content">
    ]]

    -- Turn the template into the specific HTML for this admonition style.
    local pre_html = pre_template:gsub('{{<name>}}', admonition.name):gsub('{{<Title>}}', admonition.title)

    -- The cell/row/table/div block needs to get closed out with a bunch of closure tags.
    local post_html = "</td></tr></table></div>"

    -- The admonition will be the the pre & post HTML with the admonition content in between.
    return {pandoc.RawBlock('html', pre_html), content, pandoc.RawBlock('html', post_html)}

end

-- Check the passed paragraph to see if we are dealing with an admonition.
local function process_para(el)
    -- Grab the first word from the paragraph.
    local content = el.content
    local first = content[1].text
    if first then
        -- If that first word is all upper-case and followed by a colon we *may* have an admonition
        local possible_admonition = first:match("^(%u*):")
        if possible_admonition then
            -- Key has the correct form but do we recognize it?
            local admonition = admonitions[possible_admonition]
            -- Yes it is an admonition
            if admonition then
                -- Remove the key and a likely space character thereafter to get the admonition content
                local start = 2
                if #content > 1 and content[2].t == 'Space' then start = 3 end
                local admonition_content = { table.unpack(content, start) }
                return process_admonition(admonition, admonition_content)
            end
        end
    end
end

return { Para = process_para }