/**
 * Converts a varspc to a valid xml
 *
 * The expected format for the varspc is the one produced by the xml2varspc function.
 * That it to say:
 * - varspc('foo', varspc('bar', 'baz')) will generate <foo><bar>baz</bar></baz>
 * - varspc('foo', { varspc('bar1', 'baz1'), varspc('bar2', 'baz2') }) will generate <foo><bar1>baz1</bar1></foo><foo><bar2>baz2</bar2></foo>
 *
 * @param {varspc} input varspc
 * @param {bool} if you enable the beautify function, it will includes line breaks and tabs (\n and \t) to enhance human readability, if it disabled, it will print a signle-line xml ; enabled by default
 *
 * @return {string} xml
 *
 * @public
 */
export function xml = varspc2xml(v, beautify)
{
    if (nargin < 2) // we enable the beautify parameter by default
    {
        beautify = 1;
    }

    if (!strcmp(getType(v), "char"))
    {
        xml = v;
        return;
    }

    xml = '';
    fields = v.name;

    i = for (fields) // we list every tags of the level
    {
        f = getVar(v, fields.(i));

        if (!strcmp(getType(f), "varspc")) // we found a varspc, we create a tag and we call recursively the function
        {
            xml = [xml, '<', char(fields.(i)), '>', beautify ? "\n" : "", varspc2xml(f, beautify), '</', char(fields.(i)), '>', beautify ? "\n" : ""];
        }
        else if (!strcmp(getType(f), "obj")) // we found a cell, it is meant to contain several varspc
        {
            j = for (f)
            {
                xml = [xml, '<', char(fields.(i)), '>', beautify ? "\n" : "", varspc2xml(f.{j}, beautify), '</', char(fields.(i)), '>', beautify ? "\n" : ""];
            }
        }
        else // it is just a value, we create the tag
        {
            strVal = isNumeric(f) ? char(num2str(f)) : char(f);

            xml = [xml, beautify ? "\t" : "", '<', char(fields.(i)), '>', strVal, '</', char(fields.(i)), '>', beautify ? "\n" : ""];
        }
    }
}

/**
 * Converts a xml to a varspc
 *
 * The function does not support a full xml specs
 * <? ?> headers are treated as comments just like <!-- -->
 *
 * If the same tag is found at the same level, it will encapsulate the these ones into a cell. Mind checking the type if having the same tag several times at the same level is possible in your xml.
 * For exemple: <planes><plane>airbus</plane><plane>boeing</plane></planes> will generate varspc('planes', varspc('plane', { 'airbus', 'boeing' }))
 *
 * @param {string} input xml
 *
 * @return {varspc} output varspc
 *
 * @public
 */
export function res = xml2varspc(xml)
{
    res = varspc();

    if (strcmp(getType(xml), 'char'))
    {
        logError('[xml2varspc] input is of type %s, string expected', getType(xml));
        return;
    }

    xml = removeXMLCommentsAndHeaders(xml);

    tags = listTagEvents(xml);
    nbTags = size(tags);

    skipNext = 0;

    i = for (tags) // we go through every tags
    {
        if (skipNext)
        {
            skipNext = skipNext - 1;
            continue;
        }

        tag = tags.{i};

        if ((i+1) < nbTags && tag.{2} == 1 && tags.{i+1}.{2} == -1 && !strcmp(tag.{0}, tags.{i+1}.{0})) // it means it is a value
        {
            value = xml.(lin(tag.{1} + size(['<', tag.{0}, '>']), tags.{i+1}.{1}-1));

            // maybe a tag with the same name already exists at the same level
            // in that way, we store the data into a cell
            if (isvar(res, tag.{0}))
            {
                // if the cell already exists, we append the data into it
                if (!strcmp(getType(getVar(res, tag.{0})), "obj"))
                {
                    cellToEdit = getVar(res, tag.{0});
                    cellToEdit.{end+1} = value;
                    setVar(res, tag.{0}, cellToEdit);
                }
                else // if it does not exists yet, we create it with two elements
                {
                    cellToEdit = { getVar(res, tag.{0}), value };
                    setVar(res, tag.{0}, cellToEdit);
                }
            }
            else // it does not exist, we simply store it into a varspc
            {
                setVar(res, tag.{0}, value);
            }

            skipNext = 1;
        }
        else if ((i+1) < nbTags && tag.{2} == 1) // it means it's an opening tag
        {
            posOpened = tag.{1} + size(['<', tag.{0}, '>']);

            // the idea here, is to find the associated closing tag
            // and to call the function recursively in the xml between these tags
            counter = 1;
            j = i;
            while (counter)
            {
                j = j + 1;
                if (!strcmp(tags.{j}.{0}, tag.{0}))
                {
                    counter = counter + tags.{j}.{2};
                }
                skipNext = skipNext + 1;
            }

            posClosed = tags.{j}.{1} - 1;

            subxml = xml.(lin(posOpened, posClosed));
            subvarspc = xml2varspc(subxml);

            // maybe a tag with the same name already exists at the same level
            // in that way, we store the data into a cell
            if (isvar(res, tag.{0}))
            {
                // if the cell already exists, we append the data into it
                if (!strcmp(getType(getVar(res, tag.{0})), "obj"))
                {
                    cellToEdit = getVar(res, tag.{0});
                    cellToEdit.{end+1} = subvarspc;
                    setVar(res, tag.{0}, cellToEdit);
                }
                else // if it does not exists yet, we create it with two elements
                {
                    cellToEdit = { getVar(res, tag.{0}), subvarspc };
                    setVar(res, tag.{0}, cellToEdit);
                }
            }
            else // it does not exist, we simply store it into a varspc
            {
                setVar(res, tag.{0}, subvarspc);
            }
        }
        else if (tag.{2} == -1) // it is a closing tag
        {
            // we do nothing
        }
    }
}

/**
 * Remove the comments and the xml header
 *
 * This function returns an xml without the <!-- abcef --> and <? ejklm ?>
 *
 * @param {string} input xml
 *
 * @return {varspc} output varspc
 *
 * @public
 */
export function res = removeXMLCommentsAndHeaders(xml)
{
    while (size(strfind(xml, "<!--"))) // this part could be optimised, here we run the function at complexity of O(n*(3*a+1) + n*(3*b+1)) with n: the length of the string, a: the number of comments, b: the number of headers and actually we could run it at a complexity of O(n*log(n)) I think
    {
        posOpen = strfind(xml, "<!--").(0);
        posClose = strfind(xml, "-->").(0);

        if (!size(posOpen) || !size(posClose))
        {
            logError("[XML] Invalid XML");
            res = '';
            return;
        }

        xml = [xml.(lin(0, posOpen-1)), xml.(lin(posClose + size("-->"), end))];
    }

    while (size(strfind(xml, "<?")))
    {
        posOpen = strfind(xml, "<?").(0);
        posClose = strfind(xml, "?>").(0);

        if (!size(posOpen) || !size(posClose))
        {
            logError("[XML] Invalid XML");
            res = '';
            return;
        }

        xml = [xml.(lin(0, posOpen-1)), xml.(lin(posClose + size("?>"), end))];
    }

    res = xml;
}


/**
 * This function lists every tags (opening and closing) of an xml string
 *
 * Every tag is represented by a cell wich contains the following info:
 * { tagName (string), position in the xml (int), +1 if it's an opening tag or -1 if it's a closing tag (int) }
 *
 * @param {string} input xml
 *
 * @return {cell} output cell which contains a list of cells wich contains data about the tags
 *
 * @public
 */
function tags = listTagEvents(xml)
{
    i = 0;
    xmlLength = size(xml);
    tags = {};

    currentTagName = '';
    nextTag = 0; // 1 = opening, -1 = closing
    posLastTag = 0;

    while (i < xmlLength)
    {
        if (xml.(i) == '<')
        {
            posLastTag = i;
            i = i + 1;
            if (i >= size(xml))
            {
                logError("[listTagEvents] Invalid xml file");
                return;
            }
            nextTagType = (xml.(i) == '/') ? -1 : 1;
            if (nextTagType == -1)
            {
                i = i + 1;
            }
            currentTagName = '';

            while (xml.(i) != '>' && i < xmlLength)
            {
                currentTagName.(end+1) = xml.(i);
                i = i + 1;
            }

            tags.{end+1} = { char(currentTagName), posLastTag, nextTagType };
        }

        i = i + 1;
    }
}